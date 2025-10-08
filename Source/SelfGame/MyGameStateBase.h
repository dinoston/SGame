#pragma once

#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

UCLASS()
class SELFGAME_API AMyGameState : public AGameStateBase
{
    GENERATED_BODY()
public:
    // 예: 라운드, 남은 시간 등의 공용 변수들
    UPROPERTY(BlueprintReadWrite, Replicated)
    int32 Round = 1;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const override;
};