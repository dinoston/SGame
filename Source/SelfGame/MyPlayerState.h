#pragma once

#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

UCLASS()
class SELFGAME_API AMyPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, Replicated)
    float Health = 100.f;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const override;
};