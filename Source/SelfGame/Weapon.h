
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"        // UNiagaraSystem
#include "Sound/SoundBase.h"      // USoundBase
#include "Weapon.generated.h"


class USkeletalMeshComponent;
class UNiagaraSystem;
class USoundBase;
class UDamageType;



UCLASS()
class SELFGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

	/** 무기 메시 (총구 소켓 이름: Muzzle 권장) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	ACharacter* OwnerChar;

public:	
	virtual void Tick(float DeltaTime) override;

	// ===== Stats =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float Damage = 25.f;



	/** 사거리 (히트스캔 전용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float Range = 10000.f;

	// 초당 발사수(0이면 제한 없음)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float FireRate = 8.f;

	// 탄약(−1이면 무한)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	int32 Ammo = -1;

	// ===== Sockets / FX =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = TEXT("Muzzle");

	// 머즐 플래시(원하면 UParticleSystem*로 교체 가능)
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* MuzzleFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	USoundBase* FireSound = nullptr;

	// 충돌 지점 이펙트(선택)
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* ImpactFX = nullptr;

	// 프로젝트타일 무기라면 지정(없으면 히트스캔)
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;



	/** 발사 (파생에서 override) */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();
	
	/** 메시/오너 Getter (필요시 사용) */
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	

};
