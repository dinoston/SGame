
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
	// 투사체 쪽(예: AProjectile)에서 이동/충돌 시 HealthComponent 찾아 TakeDamage(Damage) 호출

}
