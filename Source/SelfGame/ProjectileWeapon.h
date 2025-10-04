
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

UCLASS()
class SELFGAME_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	/** ������ ����ü Ŭ���� (���߿� AProjectile ���� ����) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile")
	TSubclassOf<AActor> ProjectileClass;

	virtual void Fire() override;

//public:	
//	AProjectileWeapon();
//
//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void Tick(float DeltaTime) override;

};
