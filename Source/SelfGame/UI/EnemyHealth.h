#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealth.generated.h"

class UProgressBar;
class UHealthComponent;

UCLASS()
class SELFGAME_API UWBP_EnemyHealth : public UUserWidget
{
    GENERATED_BODY()

public:
    // 적의 HealthComponent를 넘겨 바인딩/해제
    UFUNCTION(BlueprintCallable) 
    void BindToHealth(UHealthComponent* HC);

    UFUNCTION(BlueprintCallable) 
    void UnbindFromHealth();

protected:
    // UMG 디자이너에서 이름을 PB_Health로 만들어 두기
    UPROPERTY(meta = (BindWidget))
    UProgressBar* PB_EnemyHealth = nullptr;

    // 현재 바인딩된 헬스
    UPROPERTY()
    UHealthComponent* BoundHealth = nullptr;

    // 델리게이트 콜백
    UFUNCTION()
    void OnHealthRatioChanged(float NewRatio);

    // 위젯 파괴시 구독 해제
    virtual void NativeDestruct() override;

    // 프로그레스바에 반영
    void ApplyRatio(float Ratio);
};