#pragma once
#include "CoreMinimal.h"
#include "BaseCharacter.h"               // 경로 맞게
#include "Components/WidgetComponent.h"   // ← 반드시!
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
class UHealthComponent;
class UWBP_EnemyHealth;
class AEnemyAIController;


UCLASS()
class SELFGAME_API AEnemyCharacter : public ABaseCharacter
{
    GENERATED_BODY()
public:
    AEnemyCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    class UBehaviorTree* BehaviorTreeAsset;

protected:
    virtual void BeginPlay() override;
    virtual FVector GetPawnViewLocation() const override; // (옵션) 카메라 제거 시 시야 보정

    // 머리 위 체력바용 위젯 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* WC_Health = nullptr;


    UFUNCTION()
    void OnEnemyDeath();
   
};