

#include "EnemyAIController.h"
#include "../Character/EnemyCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyAIController::AEnemyAIController()
{
    bAttachToPawn = true;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    EnemyChar = Cast<AEnemyCharacter>(InPawn);
    if (!EnemyChar)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("EnemyAIController: Possessed pawn is NOT AEnemyCharacter"));
        return;
    }

    // 혹시라도 안 들고 있는 경우를 대비해서 권총 장착 보장
    EnemyChar->EquipWeaponType(EWeaponType::Pistol);

    if (!EnemyChar->BehaviorTreeAsset)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("EnemyAIController: BehaviorTreeAsset is NULL on %s"),
            *EnemyChar->GetName());
        return;
    }

    UBehaviorTree* BT = EnemyChar->BehaviorTreeAsset;
    UBlackboardComponent* BBComp = nullptr;

    // 블랙보드 초기화 + BT 실행
    if (UseBlackboard(BT->BlackboardAsset, BBComp))
    {
        this->RunBehaviorTree(BT);   // ★ 멤버 함수라서 this-> 로 호출
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("EnemyAIController: Failed to init blackboard for %s"),
            *EnemyChar->GetName());
    }
}