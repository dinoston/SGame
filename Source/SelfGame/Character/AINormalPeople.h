// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AINormalPeople.generated.h"

class UHealthComponent;
class UDamageType;
class AController;

UCLASS()
class SELFGAME_API AAINormalPeople : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAINormalPeople();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UHealthComponent> HealthComp;

	UFUNCTION() 
	void OnAnyDamage(AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);


	UFUNCTION() 
	void OnDead(); // 죽었을 때 처리(애니, 제거 등)

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
