#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"             // ★ 네가 만든 베이스 캐릭터
#include "AINormalPeople.generated.h"


class UBehaviorTree;

/**
 * 걷고 뛰고, 총 맞으면 죽기만 하는 단순 AI용 캐릭터
 * - Health / Damage / Die 는 전부 ABaseCharacter에서 재사용
 * - 여기서는 기본 능력치와 무기/UI 제거만 처리
 */
UCLASS()
class SELFGAME_API AAINormalPeople : public ABaseCharacter
{
    GENERATED_BODY()

public:
    AAINormalPeople();

protected:
    virtual void BeginPlay() override;
    

public:
    // 나중에 BT에서 쓸 플래그
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    bool bIsPanic = false;

public:
    // 이 AI가 사용할 Behavior Tree (BP_AINormalPeople에서 지정)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset = nullptr;

    virtual float TakeDamage(float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator,
        class AActor* DamageCauser) override;
};
