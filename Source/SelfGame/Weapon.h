
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"      // USoundBase
#include "Weapon.generated.h"


class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;
class UDamageType;
class ACharacter;


UCLASS()
class SELFGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

	/** 무기 메시 (총구 소켓 이름: MuzzleFlash) */
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

	

	/** 대미지 타입(선택) */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Stats")
	TSubclassOf<UDamageType> DamageType;

	// ===== Sockets / FX =====
	 /** 메시의 총구 소켓 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = TEXT("MuzzleFlash"); // 메시에 맞게 바꾸기

	/** 머즐 플래시 Niagara */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* MuzzleFX = nullptr;

	/** 발사 사운드 */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	USoundBase* FireSound = nullptr;

	// 충돌 지점 이펙트(선택)
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* ImpactFX = nullptr;

	/** 충돌 사운드(선택) */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	USoundBase* ImpactSound = nullptr;

	/** ProjectileClass가 있으면 투사체, 없으면 히트스캔 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;



	/** 발사 (파생에서 override) */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();
	
	/** 메시/오너 Getter (필요시 사용) */
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	

	//총알//
	//Ammo 변수는 이미 존재합니다. 여기에 MaxAmmo와 AddAmmo만 추가합니다. (기존 Ammo 정의는 그대로 사용하되 초기값만 30으로 맞추면 됩니다.)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	int32 MaxAmmo = 70;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	int32 Ammo = 30;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	int32 AddAmmo(int32 Amount);


private:
	/** 연사 쿨다운용 */
	double LastFireTime = -1.0;

	/** 총구 트랜스폼 얻기(없으면 메시 기준) */
	FTransform GetMuzzleTransform() const;

	/** 머즐 FX/사운드 재생 */
	void PlayMuzzleFX(const FTransform& MuzzleXf) const;

	/** 임팩트 FX/사운드 재생 */
	void PlayImpactFX(const FHitResult& Hit) const;

	/** 지금 발사 가능? (FireRate, Ammo 체크) */
	bool CanFireNow() const;

};
