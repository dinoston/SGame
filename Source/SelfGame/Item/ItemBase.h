// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class USphereComponent;
class UStaticComponent;
class ABaseCharacter;

UCLASS()
class SELFGAME_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();

protected:
	virtual void BeginPlay() override;

	/** 오버랩 감지용 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USphereComponent* Sphere;

	/** 보기용 메시(충돌 없음) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* Mesh;

	/** 플레이어가 닿았을 때 호출되는 콜백 */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 실제 효과 적용(파생에서 구현) */
	virtual void ApplyTo(class ABaseCharacter* Character);

};
