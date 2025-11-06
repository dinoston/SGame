#include "NormalPeopleAIController.h"
#include "../Character/AINormalPeople.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ANormalPeopleAIController::ANormalPeopleAIController()
{
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void ANormalPeopleAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AAINormalPeople* NPC = Cast<AAINormalPeople>(InPawn);
    if (!NPC)
    {
        UE_LOG(LogTemp, Warning, TEXT("NormalPeopleAIController: Not an AINormalPeople"));
        return;
    }

    if (NPC->BehaviorTreeAsset == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("NormalPeopleAIController: BehaviorTreeAsset is null on %s"), *NPC->GetName());
        return;
    }

    // Behavior Tree ½ÇÇà
    if (RunBehaviorTree(NPC->BehaviorTreeAsset))
    {
        UBlackboardComponent* BB = GetBlackboardComponent();
        if (BB && NPC->BehaviorTreeAsset->BlackboardAsset)
        {
            BB->InitializeBlackboard(*(NPC->BehaviorTreeAsset->BlackboardAsset));
            BlackboardComp = BB;
        }

        UE_LOG(LogTemp, Log, TEXT("NormalPeopleAIController: BT started on %s"), *NPC->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NormalPeopleAIController: Failed to RunBehaviorTree on %s"), *NPC->GetName());
    }
}