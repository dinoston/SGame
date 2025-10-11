#include "BaseCharacter.h"
#include "../HealthComponent.h"
#include "InputCoreTypes.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "../Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"


#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{

    HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    // PrimaryActorTick.bCanEverTick = true; // 필요하면 켜세요

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = true;                          // 스프링암이 컨트롤러 회전 따라감

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false;                            // 카메라는 암을 따름


    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true; // 부드러운 회전
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);



    //점프 속성
    GetCharacterMovement()->JumpZVelocity = 420.f;   // 점프 높이 (기본 420)
    GetCharacterMovement()->AirControl = 0.35f;   // 공중에서 방향 전환

}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();


    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;   // 기본은 걷기 속도


    // … 기존 PrimaryWeapon/SecondaryWeapon 스폰 + AttachWeapon(W) 호출 …

    //// 시작은 맨손(또는 원하는 시작 무기)
    //CurrentWeaponType = EWeaponType::None;
    //CurrentWeapon = nullptr;
    // 만약 시작부터 A를 들게 하고 싶다면:
    // EquipWeaponType(EWeaponType::Pistol);  // 네 enum 값 이름에 맞춰서







    // ── 무기 스폰 + 손 소켓에 부착 ──
    FActorSpawnParameters Params;
    Params.Owner = this; // 발사체/무기에서 GetOwner()로 캐릭터 추적 가능
    Params.Instigator = this;  // 데미지 시스템에서 InstigatorController 추적 용이

    if(PistolWeaponClass)
    {
        PistolWeapon = GetWorld()->SpawnActor<AWeapon>
            (PistolWeaponClass, GetActorLocation(), GetActorRotation(), Params);
    }

    if (RifleWeaponClass)
    {
        RifleWeapon = GetWorld()->SpawnActor<AWeapon>
            (RifleWeaponClass, GetActorLocation(), GetActorRotation(), Params);
        if (RifleWeapon)
        {
            // 보조 무기는 처음엔 부착하지 않고 대기(원하면 Hidden 설정 가능)
            RifleWeapon->SetActorHiddenInGame(true);
            RifleWeapon->SetActorEnableCollision(false);

        }
    }

    // 시작 장착(권총) — enum까지 동기화되도록 타입 함수로 호출
    
    if (PistolWeapon)
    {
        EquipWeaponType(EWeaponType::Pistol);
    }
    else
    {
        CurrentWeaponType = EWeaponType::None;
        CurrentWeapon = nullptr;
    }

}

void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


// ★ 컨트롤러가 모든 인풋 바인딩을 담당하므로 비워둠
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    //걷기 뛰기//
    if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (IA_Running)
        {
            EIC->BindAction(IA_Running, ETriggerEvent::Started, this, &ABaseCharacter::OnRunPressed);
            EIC->BindAction(IA_Running, ETriggerEvent::Completed, this, &ABaseCharacter::OnRunReleased);
            EIC->BindAction(IA_Running, ETriggerEvent::Canceled, this, &ABaseCharacter::OnRunReleased);
        }
    }

}

void ABaseCharacter::OnRunPressed(const FInputActionValue& Value)
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

}

void ABaseCharacter::OnRunReleased(const FInputActionValue& Value)
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}



/* ---------- Weapon ---------- */


void ABaseCharacter::EquipWeaponType(EWeaponType NewType)
{
    // 1) 전부 숨김/비활성
    if (PistolWeapon)
    {
        PistolWeapon->SetActorHiddenInGame(true);
        PistolWeapon->SetActorEnableCollision(false);
        PistolWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }
    if (RifleWeapon)
    {
        RifleWeapon->SetActorHiddenInGame(true);
        RifleWeapon->SetActorEnableCollision(false);
        RifleWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }
    CurrentWeapon = nullptr;

    // 2) 타입 선택 → 부착
    if (NewType == EWeaponType::Pistol && PistolWeapon)
    {
        CurrentWeapon = PistolWeapon;
        AttachWeapon(CurrentWeapon);
    }
    else if (NewType == EWeaponType::Rifle && RifleWeapon)
    {
        CurrentWeapon = RifleWeapon;
        AttachWeapon(CurrentWeapon);
    }


    // 3) 현재 타입 갱신 → AnimBP에서 이 값으로 포즈 전환
    CurrentWeaponType = NewType;
}

void ABaseCharacter::UnEquipAll()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentWeapon = nullptr;

    }
    CurrentWeaponType = EWeaponType::None;
}



void ABaseCharacter::AttachWeapon(AWeapon* W)
{
    if (!IsValid(W) || !GetMesh()) return;

    W->SetOwner(this);
    W->SetInstigator(this);

    // 소켓 존재 확인
    if (!GetMesh()->DoesSocketExist(WeaponSocketName))
    {
        UE_LOG(LogTemp, Error, TEXT("Socket '%s' not found on %s"),
            *WeaponSocketName.ToString(), *GetNameSafe(GetMesh()));
        return;
    }

    // ✨핵심: 소켓로 '스냅'해서 부착 (스폰 위치/오프셋 무시)
    W->AttachToComponent(
        GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        WeaponSocketName              // <- 하드코딩 "WeaponSocket" 금지
    );

    // 들고 있을 땐 충돌/물리 OFF
    W->SetActorHiddenInGame(false);
    W->SetActorEnableCollision(false);
    if (auto* Prim = Cast<UPrimitiveComponent>(W->GetRootComponent()))
    {
        Prim->SetSimulatePhysics(false);
    }
}

void ABaseCharacter::FirePressed()
{
    if (!IsValid(CurrentWeapon)) return;

    // AWeapon에 Fire가 UFUNCTION(BlueprintCallable) 또는 C++ 메서드로 있다고 가정
    // CurrentWeapon->Fire();

    // 만약 Fire가 아직 없다면, AWeapon에 아래 시그니처를 추가해줘:
    // UFUNCTION(BlueprintCallable) virtual void Fire();
}

void ABaseCharacter::EquipPistol()
{
    if (!IsValid(PistolWeapon) || CurrentWeapon == PistolWeapon) return;

    // 현재 무기 숨기고 충돌 비활성화(선택)
    if (IsValid(CurrentWeapon))

    {
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }
    CurrentWeapon = PistolWeapon;
    AttachWeapon(CurrentWeapon);

}

void ABaseCharacter::EquipRifle()
{
    if (!IsValid(RifleWeapon) || CurrentWeapon == RifleWeapon) return;

    if (IsValid(CurrentWeapon))
    {
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }

    CurrentWeapon = RifleWeapon;
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

