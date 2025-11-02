// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealth.h"
#include "Components/ProgressBar.h"
#include "../HealthComponent.h"

void UWBP_EnemyHealth::BindToHealth(UHealthComponent* HC)
{
    if (!HC) return;
    if (BoundHealth == HC) return;

    UnbindFromHealth();

    HC->OnHealthRatioChanged.AddDynamic(this, &UWBP_EnemyHealth::OnHealthRatioChanged);
    BoundHealth = HC;

    // 초기 값 바로 반영
    ApplyRatio(HC->GetHealthRatio());
}

void UWBP_EnemyHealth::UnbindFromHealth()
{
    if (BoundHealth)
    {
        BoundHealth->OnHealthRatioChanged.RemoveDynamic(this, &UWBP_EnemyHealth::OnHealthRatioChanged);
        BoundHealth = nullptr;
    }
}

void UWBP_EnemyHealth::OnHealthRatioChanged(float NewRatio)
{
    ApplyRatio(NewRatio);
}

void UWBP_EnemyHealth::ApplyRatio(float Ratio)
{
    if (PB_EnemyHealth)
    {
        PB_EnemyHealth->SetPercent(FMath::Clamp(Ratio, 0.f, 1.f));
    }
}

void UWBP_EnemyHealth::NativeDestruct()
{
    UnbindFromHealth();
    Super::NativeDestruct();
}