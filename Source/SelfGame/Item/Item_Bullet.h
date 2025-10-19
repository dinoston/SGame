// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Item_Bullet.generated.h"

/**
 * 
 */
UCLASS()
class SELFGAME_API AItem_Bullet : public AItemBase
{
	GENERATED_BODY()
	

public:
	// 한 번 먹을 때 채워줄 탄약 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 GiveAmount = 30;


	//먹는 소리(에디터에서 사운드 큐/사운드 지정)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	USoundBase* PickupSound = nullptr;
private:
	virtual void ApplyTo(class ABaseCharacter* Character) override;

};
