
#include "ProjectileWeapon.h"
#include "Engine/World.h"

//AProjectileWeapon::AProjectileWeapon()
//{
//	PrimaryActorTick.bCanEverTick = true;
//
//}
//
//void AProjectileWeapon::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//void AProjectileWeapon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AProjectileWeapon::Fire()
{
	if (!ProjectileClass) return;

	const FVector SpawnLoc =
		(GetMesh() && GetMesh()->DoesSocketExist(MuzzleSocketName)) ? GetMesh()->GetSocketLocation(MuzzleSocketName)
		: GetActorLocation();

	const FRotator SpawnRot = GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLoc, SpawnRot, Params);
	// ����ü ��(��: AProjectile)���� �̵�/�浹 �� HealthComponent ã�� TakeDamage(Damage) ȣ��

}
