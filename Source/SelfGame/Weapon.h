
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

	/** ���� �޽� (�ѱ� ���� �̸�: Muzzle ����) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	ACharacter* OwnerChar;

public:	
	virtual void Tick(float DeltaTime) override;

	/** �� �� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float Damage = 25.f;

	/** ��Ÿ� (��Ʈ��ĵ ����) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float Range = 10000.f;

	/** �ѱ� ���� �̸� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = TEXT("Muzzle");

	/** �߻� (�Ļ����� override) */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();
	
	/** �޽�/���� Getter (�ʿ�� ���) */
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	

};
