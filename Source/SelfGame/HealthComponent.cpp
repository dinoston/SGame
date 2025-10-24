#include "HealthComponent.h"
#include "Engine/Engine.h" // (선택) GEngine 쓰실 거면 유지, 아니면 제거 가능

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    // ★ 시작 체력을 MaxHP 범위로 클램프해서 적용
    CurrentHP = FMath::Clamp(StartHP, 0.f, MaxHP);

    // ★ 추가: 초기 표시용 알림
    OnHealthRatioChanged.Broadcast(GetHealthRatio());

    // (선택) 시작값 화면에 한번 표시하고 싶으면 주석 해제
    // if (GEngine)
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan,
    //         FString::Printf(TEXT("HP Start: %.0f / %.0f"), CurrentHP, MaxHP));
    // }
}

void UHealthComponent::TakeDamage(float Amount)
{
    if (CurrentHP <= 0.f) return;

    Amount = FMath::Max(0.f, Amount);
    if (Amount <= KINDA_SMALL_NUMBER) return;

    const float Old = CurrentHP;
    CurrentHP = FMath::Clamp(Old - Amount, 0.f, MaxHP);
    const float ActuallyDealt = Old - CurrentHP; // 실제 깎인 양

    // ★ 화면 디버그(-값, 현재/최대)
    if (GEngine && ActuallyDealt > 0.f)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 1.2f, FColor::Red,
            FString::Printf(TEXT("-%.0f HP  (%.0f / %.0f)"),
                ActuallyDealt, CurrentHP, MaxHP));
    }

    // ★ 추가: 변경 알림
    OnHealthRatioChanged.Broadcast(GetHealthRatio());

    if (CurrentHP <= 0.f)
    {
        OnDeath.Broadcast();
        UE_LOG(LogTemp, Error, TEXT("You are dead!!"));
    }
}


float UHealthComponent::Heal(float Amount)
{
    if (CurrentHP <= 0.f) return 0.f;           // 죽은 상태면 회복 X (원하면 허용 가능)
    Amount = FMath::Max(0.f, Amount);
    if (Amount <= KINDA_SMALL_NUMBER) return 0.f;

    const float Old = CurrentHP;
    CurrentHP = FMath::Clamp(Old + Amount, 0.f, MaxHP);
    const float ActuallyHealed = CurrentHP - Old;

    // ★ 화면 디버그(+값, 현재/최대)
    if (GEngine && ActuallyHealed > 0.f)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 1.2f, FColor::Green,
            FString::Printf(TEXT("+%.0f HP  (%.0f / %.0f)"),
                ActuallyHealed, CurrentHP, MaxHP));
    }

    // ★ 추가: 변경 알림
    OnHealthRatioChanged.Broadcast(GetHealthRatio());

    return ActuallyHealed;
}