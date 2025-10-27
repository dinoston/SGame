// Fill out your copyright notice in the Description page of Project Settings.


#include "WBP_UI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Components/Image.h"
#include "GameFramework/Pawn.h"
#include "../HealthComponent.h"
#include "../Weapon.h" // ★ 무기 포함
#include "../Character/BaseCharacter.h"




void UWBP_UI::InitFromPawn(APawn* Pawn)
{
    // 기존 바인딩 해제
    UnbindFromHealth();
    UnbindFromWeapon();
    UnbindFromCharacter();



    if (!Pawn) return;

    if (UHealthComponent* HC = Pawn->FindComponentByClass<UHealthComponent>())
    {
        BindToHealth(HC);
        ApplyRatioToUI(HC->GetHealthRatio()); // 초기값 반영
    }

    if (auto* C = Cast<ABaseCharacter>(Pawn))
    {
        BindToCharacter(C); // 무기 교체 이벤트 구독

        if (C->CurrentWeapon)                 // 현재 무기 있으면 즉시 표시
        {
            BindToWeapon(C->CurrentWeapon);
            ApplyAmmoToUI(C->CurrentWeapon->GetAmmo(), C->CurrentWeapon->GetMaxAmmo());
        }
        else if (Text_Ammo)
        {
            Text_Ammo->SetText(FText::FromString(TEXT("-- / --")));
        }
    }
}

void UWBP_UI::UpdateCountdown(float SecondsLeft)
{
    if (!Text_Countdown) return;

    // 00:00 형식
    const int32 S = FMath::Max(0, FMath::RoundToInt(SecondsLeft));
    const int32 M = S / 60;
    const int32 R = S % 60;

    Text_Countdown->SetText(
        FText::FromString(FString::Printf(TEXT("%02d:%02d"), M, R))
    );
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

//weapon********************************************************//
void UWBP_UI::BindToWeapon(AWeapon* Wpn)
{
    if (!Wpn) return;
    if (BoundWeapon == Wpn) return;
    UnbindFromWeapon();

    // 델리게이트 구독
    Wpn->OnAmmoChanged.AddDynamic(this, &UWBP_UI::OnAmmoChanged);
    BoundWeapon = Wpn;
}

void UWBP_UI::UnbindFromWeapon()
{
    if (BoundWeapon)
    {
        BoundWeapon->OnAmmoChanged.RemoveDynamic(this, &UWBP_UI::OnAmmoChanged);
        BoundWeapon = nullptr;
    }
}

void UWBP_UI::OnAmmoChanged(int32 NewAmmo, int32 NewMax)
{
    ApplyAmmoToUI(NewAmmo, NewMax);
}

void UWBP_UI::ApplyAmmoToUI(int32 Ammo, int32 MaxAmmo)
{
    if (Text_Ammo)
    {
        Text_Ammo->SetText(FText::FromString(
            FString::Printf(TEXT("%d / %d"), Ammo, MaxAmmo)
        ));
    }
}

void UWBP_UI::BindToCharacter(ABaseCharacter* C)
{
    if (!C || BoundCharacter == C) return;
    UnbindFromCharacter();
    C->OnCurrentWeaponChanged.AddDynamic(this, &UWBP_UI::OnCurrentWeaponChanged);
    BoundCharacter = C;
}

void UWBP_UI::UnbindFromCharacter()
{
    if (BoundCharacter)
    {
        BoundCharacter->OnCurrentWeaponChanged.RemoveDynamic(this, &UWBP_UI::OnCurrentWeaponChanged);
        BoundCharacter = nullptr;
    }
}

void UWBP_UI::OnCurrentWeaponChanged(AWeapon* NewWeapon)
{
    BindToWeapon(NewWeapon);
    if (NewWeapon) ApplyAmmoToUI(NewWeapon->GetAmmo(), NewWeapon->GetMaxAmmo());
    else if (Text_Ammo) Text_Ammo->SetText(FText::FromString(TEXT("-- / --")));
}



void UWBP_UI::NativeDestruct()
{
    UnbindFromHealth();
    UnbindFromWeapon();  // ★ 탄약 구독 해제 추가
    UnbindFromCharacter(); // ★ 추가

    Super::NativeDestruct();
}