#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalPeopleAIController.generated.h"

class UBlackboardComponent;

UCLASS()
class SELFGAME_API ANormalPeopleAIController : public AAIController
{
    GENERATED_BODY()

public:
    ANormalPeopleAIController();

    virtual void OnPossess(APawn* InPawn) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UBlackboardComponent* BlackboardComp;


};