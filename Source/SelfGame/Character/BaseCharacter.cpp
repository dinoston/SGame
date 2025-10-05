#include "BaseCharacter.h"
#include "../HealthComponent.h"
#include "InputCoreTypes.h"
#include "Engine/Engine.h"
#include "../Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

ABaseCharacter::ABaseCharacter()
{
    HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    // PrimaryActorTick.bCanEverTick = true; // 필요하면 켜세요

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 700.f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);


}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    // ── 무기 스폰 + 손 소켓에 부착 ──
    FActorSpawnParameters Params;
    Params.Owner = this; // 발사체/무기에서 GetOwner()로 캐릭터 추적 가능
    Params.Instigator = this;  // 데미지 시스템에서 InstigatorController 추적 용이

    if(PrimaryWeaponClass)
    {
        PrimaryWeapon = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass, GetActorLocation(), GetActorRotation(), Params);
    }

    if (SecondaryWeaponClass)
    {
        SecondaryWeapon = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass, GetActorLocation(), GetActorRotation(), Params);
        if (SecondaryWeapon)
        {
            // 보조 무기는 처음엔 부착하지 않고 대기(원하면 Hidden 설정 가능)
            SecondaryWeapon->SetActorHiddenInGame(true);
            SecondaryWeapon->SetActorEnableCollision(false);

        }
    }

    // 기본 장착: 1번 무기
    if (PrimaryWeapon)
    {
        CurrentWeapon = PrimaryWeapon;
        AttachWeapon(CurrentWeapon);
    }

}

void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (IA_Move) EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABaseCharacter::OnMove);
        if (IA_Look) EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABaseCharacter::OnLook);

        if (IA_Jump)
        {
            EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ABaseCharacter::OnJumpPressed);
            EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABaseCharacter::OnJumpReleased);
            EIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ABaseCharacter::OnJumpPressed);

        }

    // (옵션) 기존 테스트: 0 → 50 데미지  
    PlayerInputComponent->BindKey(EKeys::Zero, IE_Pressed, this, &ABaseCharacter::TestTakeDamage);
    }   
    
}

void ABaseCharacter::AttachWeapon(AWeapon* W)
{
    if (!IsValid(W) || !GetMesh()) return;

    // 오너/인스티게이터 지정(확실하게)
    W->SetOwner(this);
    W->SetInstigator(this);

    // 소켓 이름이 실제 존재하는지 체크 후 부착
    const bool bHasSocket = GetMesh()->DoesSocketExist(WeaponSocketName);
    const FName SocketToUse = bHasSocket ? WeaponSocketName : NAME_None;

    W->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketToUse);

    // 보이기/충돌 활성화(장착 상태)
    W->SetActorHiddenInGame(false);
    W->SetActorEnableCollision(true);
}

/* ── 입력 핸들러 구현 ── */
void ABaseCharacter::OnMove(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>(); // X=Right, Y=Forward
    if (Controller)
    {
        const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
        const FVector Fwd = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
        const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
        AddMovementInput(Fwd, Axis.Y);
        AddMovementInput(Right, Axis.X);
    }

}

void ABaseCharacter::OnLook(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>(); // Mouse X/Y
    AddControllerYawInput(Axis.X);
    AddControllerPitchInput(Axis.Y);
}

void ABaseCharacter::OnJumpPressed(const FInputActionValue& Value)
{
    Jump();
}

void ABaseCharacter::OnJumpReleased(const FInputActionValue& Value)
{
    StopJumping();
}

void ABaseCharacter::FirePressed()
{
    if (!IsValid(CurrentWeapon)) return;

    // AWeapon에 Fire가 UFUNCTION(BlueprintCallable) 또는 C++ 메서드로 있다고 가정
    // CurrentWeapon->Fire();

    // 만약 Fire가 아직 없다면, AWeapon에 아래 시그니처를 추가해줘:
    // UFUNCTION(BlueprintCallable) virtual void Fire();
}

void ABaseCharacter::EquipPrimary()
{
    if (!IsValid(PrimaryWeapon) || CurrentWeapon == PrimaryWeapon) return;

    // 현재 무기 숨기고 충돌 비활성화(선택)
    if (IsValid(CurrentWeapon))

    {
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }
    CurrentWeapon = PrimaryWeapon;
    AttachWeapon(CurrentWeapon);

}

void ABaseCharacter::EquipSecondary()
{
    if (!IsValid(SecondaryWeapon) || CurrentWeapon == SecondaryWeapon) return;

    if (IsValid(CurrentWeapon))
    {
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }

    CurrentWeapon = SecondaryWeapon;
    AttachWeapon(CurrentWeapon);
}

void ABaseCharacter::TestTakeDamage()
{
    if (!HealthComp) return;

    HealthComp->TakeDamage(50.f);

    // (선택) 화면에 현재 HP 표시 — 안 써도 됨
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("HP: %.1f"), HealthComp->CurrentHP));
    }

}

