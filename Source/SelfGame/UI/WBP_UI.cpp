// Fill out your copyright notice in the Description page of Project Settings.


#include "WBP_UI.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "GameFramework/Pawn.h"
#include "../HealthComponent.h"


void UWBP_UI::InitFromPawn(APawn* Pawn)
{
    // 기존 바인딩 해제
    UnbindFromHealth();

    if (!Pawn) return;

    if (UHealthComponent* HC = Pawn->FindComponentByClass<UHealthComponent>())
    {
        BindToHealth(HC);
        ApplyRatioToUI(HC->GetHealthRatio()); // 초기값 반영
    }
}

void UWBP_UI::BindToHealth(UHealthComponent* HC)
{
    if (!HC) return;

    // 중복 방지
    if (BoundHealth == HC) return;
    UnbindFromHealth();

    HC->OnHealthRatioChanged.AddDynamic(this, &UWBP_UI::OnHealthRatioChanged);
    BoundHealth = HC;
}

void UWBP_UI::UnbindFromHealth()
{
    if (BoundHealth)
    {
        BoundHealth->OnHealthRatioChanged.RemoveDynamic(this, &UWBP_UI::OnHealthRatioChanged);
        BoundHealth = nullptr;
    }
}

void UWBP_UI::OnHealthRatioChanged(float NewRatio)
{
    ApplyRatioToUI(NewRatio);
}

void UWBP_UI::ApplyRatioToUI(float Ratio)
{
    if (PB_Health)
    {
        PB_Health->SetPercent(FMath::Clamp(Ratio, 0.f, 1.f));
    }
    // 필요하면 조준점 크기/알파를 Ratio에 따라 조절하는 것도 여기서 가능
}

void UWBP_UI::NativeDestruct()
{
    UnbindFromHealth();
    Super::NativeDestruct();
}