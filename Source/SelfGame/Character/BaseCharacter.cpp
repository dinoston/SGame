#include "BaseCharacter.h"
#include "../HealthComponent.h"
#include "InputCoreTypes.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "../Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

    //// 캐릭터 회전/이동 세팅(선호 스타일)
    //bUseControllerRotationPitch = false;
    //bUseControllerRotationRoll = false;
    //bUseControllerRotationYaw = true; // 카메라만 회전
    //GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향으로 회전
    //// (선택) 부드럽게 컨트롤러 방향으로 회전하고 싶으면
    ////GetCharacterMovement()->bUseControllerDesiredRotation = false;
    //GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // 회전 속도
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


// ★ 컨트롤러가 모든 인풋 바인딩을 담당하므로 비워둠
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
}

/* ---------- Weapon helpers ---------- */


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

