#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
    // 월드에 배치/스폰되면 자동으로 AI가 소유
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!IsPlayerControlled())
    {
        if (Camera) { Camera->DestroyComponent(); }
        if (SpringArm) { SpringArm->DestroyComponent(); }
    }
}

FVector AEnemyCharacter::GetPawnViewLocation() const
{
    static const FName HeadSocket(TEXT("head")); // 프로젝트 소켓명에 맞게 수정
    if (const USkeletalMeshComponent* M = GetMesh())
        if (M->DoesSocketExist(HeadSocket))
            return M->GetSocketLocation(HeadSocket);
    return Super::GetPawnViewLocation();
}