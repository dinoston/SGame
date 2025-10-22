
#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"

#include "Item_Health.generated.h"

class UNiagaraSystem;    // Niagara
class UParticleSystem;
class USoundBase;
class ABaseCharacter;


UCLASS()
class SELFGAME_API AItem_Health : public AItemBase
{
	GENERATED_BODY()

public:
	// 얼마를 회복할지 (에디터에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float HealAmount = 30.f;

	// HP가 이미 풀이라면 주울 수 없게 막고 싶을 때
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	bool bBlockPickupAtFullHP = false;

	// 연출(선택)
	// 1) Cascade
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	UParticleSystem* PickupVFX_Cascade = nullptr;


	// 2) Niagara
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	UNiagaraSystem* PickupVFX_Niagara = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	USoundBase* PickupSFX = nullptr;
	


protected:
	virtual void ApplyTo(class ABaseCharacter* Character) override;
};
