#include "NormalPeopleAIController.h"
#include "../Character/AINormalPeople.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ANormalPeopleAIController::ANormalPeopleAIController()
{
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

  
}