// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Bullet.h"
#include "../Character/BaseCharacter.h"
#include "../Weapon.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"   


void AItem_Bullet::ApplyTo(ABaseCharacter* Character)
{
    if (!Character || !Character->CurrentWeapon) return;

    // 무기 탄약 보급
    const int32 Added = Character->CurrentWeapon->AddAmmo(GiveAmount);

    // 보급이 실제로 되었을 때만 재생 (가득 차 있으면 생략)
    if (Added > 0 && PickupSound)
    {
        // 3D로 들리게: 아이템 위치에서 재생
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
        // 2D UI처럼 전체에 들리게 하고 싶으면:
        // UGameplayStatics::PlaySound2D(this, PickupSound);
    }


    // 화면 안내
    if (GEngine)
    {
        if (Added > 0)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan,
                FString::Printf(TEXT("+%d Ammo  (%d/%d)"),
                    Added,
                    Character->CurrentWeapon->Ammo,
                    Character->CurrentWeapon->MaxAmmo));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,
                TEXT("Ammo is already full"));
        }
    }
}
