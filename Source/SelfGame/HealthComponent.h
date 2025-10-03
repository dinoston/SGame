#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/** 필요시 BP에서 바인딩할 수 있도록 남겨둠(원치 않으면 주석 처리해도 됨) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SELFGAME_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHP = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHP = 0.f;

    /** 사망 이벤트(원하면 BP에서 연결) */
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeath OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float Amount);
};