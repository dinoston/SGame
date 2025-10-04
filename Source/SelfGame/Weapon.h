
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


class USkeletalMeshComponent;

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

	/** 한 발 데미지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float Damage = 25.f;

	/** 사거리 (히트스캔 전용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float Range = 10000.f;

	/** 총구 소켓 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = TEXT("Muzzle");

	/** 발사 (파생에서 override) */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();
	
	/** 메시/오너 Getter (필요시 사용) */
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	

};
