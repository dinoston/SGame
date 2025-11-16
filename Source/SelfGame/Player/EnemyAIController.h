// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"                 // ★ PlayerController 말고 이것만
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SELFGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	

public:
    AEnemyAIController();

    virtual void OnPossess(APawn* InPawn) override;

protected:
    // 내가 조종중인 적 캐릭터 캐시
    UPROPERTY()
    class AEnemyCharacter* EnemyChar = nullptr;
};
