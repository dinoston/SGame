#include "HealthComponent.h"
#include "Engine/Engine.h" // (����) GEngine ���� �Ÿ� ����, �ƴϸ� ���� ����

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
        // ���� �α� ǥ��
    }
}