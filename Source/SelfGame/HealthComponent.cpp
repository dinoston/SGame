#include "HealthComponent.h"
#include "Engine/Engine.h" // (선택) GEngine 쓰실 거면 유지, 아니면 제거 가능

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentHP = MaxHP;
}

void UHealthComponent::TakeDamage(float Amount)
{
    if (CurrentHP <= 0.f) return;

    CurrentHP = FMath::Clamp(CurrentHP - Amount, 0.f, MaxHP);

    if (CurrentHP <= 0.f)
    {
        OnDeath.Broadcast();
            UE_LOG(LogTemp, Error, TEXT("You are dead!!"));
        // 죽음 로그 표시
    }
}