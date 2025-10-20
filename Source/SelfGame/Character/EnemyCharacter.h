#pragma once
#include "CoreMinimal.h"
#include "BaseCharacter.h"               // 경로 맞게
#include "EnemyCharacter.generated.h"

UCLASS()
class SELFGAME_API AEnemyCharacter : public ABaseCharacter
{
    GENERATED_BODY()
public:
    AEnemyCharacter();

protected:
    virtual void BeginPlay() override;
    virtual FVector GetPawnViewLocation() const override; // (옵션) 카메라 제거 시 시야 보정
};