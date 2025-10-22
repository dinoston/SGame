// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Health.h"
#include "../Character/BaseCharacter.h"
#include "../HealthComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraFunctionLibrary.h"


void AItem_Health::ApplyTo(ABaseCharacter* Character)
{

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("ApplyTo(Health) called"));
    }
    if (!Character || !Character->HealthComp) return;

    const float Cur = Character->HealthComp->CurrentHP;
    const float Max = Character->HealthComp->MaxHP;

    if (bBlockPickupAtFullHP && Cur >= Max)
    {
        return; // 풀피면 효과 없음
    }

    // 컴포넌트의 Heal 사용(이벤트/클램프/안전장치 모두 내부에서 처리)
    const float ActuallyHealed = Character->HealthComp->Heal(HealAmount);

    // 회복이 실제로 일어났다면 FX/SFX 재생
    if (ActuallyHealed > 0.f)
    {
        // --- FX: Niagara가 있으면 우선 사용 ---
        if (PickupVFX_Niagara)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                this,
                PickupVFX_Niagara,
                GetActorLocation(),
                GetActorRotation()
                // , FVector(1.f) // 스케일 지정 가능
            );
        }
        // --- Niagara가 없고 Cascade가 있으면 Cascade 사용 ---
        else if (PickupVFX_Cascade)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                PickupVFX_Cascade,
                GetActorLocation(),
                GetActorRotation()
                // , FVector(1.f) // 스케일 지정 가능
            );
        }
        if (PickupSFX)
        {
            UGameplayStatics::PlaySoundAtLocation(this, PickupSFX, GetActorLocation());
        }
    }

    // Destroy는 AItemBase에서 Overlap 후 처리해준다고 가정.
    // 만약 AItemBase가 Destroy를 안한다면 여기서 Destroy() 호출.
    // Destroy();
}