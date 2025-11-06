#include "AINormalPeople.h"
#include "../HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Weapon.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

AAINormalPeople::AAINormalPeople()
{
    // --- 체력 설정: 한 방 혹은 몇 방에 죽게 만들 값 ---
    if (HealthComp)
    {
        HealthComp->MaxHP = 20.f;   // 여기서 바로 조절 가능
        HealthComp->StartHP = 20.f;
    }

    // --- 이동 속도: 천천히 걷고 조금 빨리 뛰는 정도 ---
    WalkSpeed = 200.f;
    SprintSpeed = 500.f;

    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->MaxWalkSpeed = WalkSpeed;
    }

    // 이 캐릭터는 플레이어 입력 안 쓰니까
    // BaseCharacter에 있는 Fire/무기 관련 함수는 그냥 안 쓰면 됨.

    // 필요하면 콜리전 채널도 여기서 맞춰줄 수 있음 (총알 채널 Block)
    // GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
    // GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void AAINormalPeople::BeginPlay()
{
    Super::BeginPlay();

    // 혹시 BaseCharacter에서 무기를 스폰했더라도,
    // 이 AI는 맨손이어야 하니 전부 해제 + 파괴해둔다.

    UnEquipAll();

    if (PistolWeapon)
    {
        PistolWeapon->Destroy();
        PistolWeapon = nullptr;
    }

    if (RifleWeapon)
    {
        RifleWeapon->Destroy();
        RifleWeapon = nullptr;
    }

    // UI도 필요 없으니, BP에서 CharacterUI를 비워두면 생성 안 됨.

    if (!IsPlayerControlled())
    {
        if (Camera) { Camera->DestroyComponent(); }
        if (SpringArm) { SpringArm->DestroyComponent(); }
    }
}
