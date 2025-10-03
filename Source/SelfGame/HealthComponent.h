#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/** �ʿ�� BP���� ���ε��� �� �ֵ��� ���ܵ�(��ġ ������ �ּ� ó���ص� ��) */
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

    /** ��� �̺�Ʈ(���ϸ� BP���� ����) */
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeath OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float Amount);
};