
#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Character/BaseCharacter.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->InitSphereRadius(50.f);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Overlap); // 겹침만
	Sphere->SetGenerateOverlapEvents(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnSphereBeginOverlap);
}

void AItemBase::OnSphereBeginOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (ABaseCharacter* C = Cast<ABaseCharacter>(OtherActor))
	{
		ApplyTo(C);
		Destroy(); // 1회성 픽업
	}
}

void AItemBase::ApplyTo(ABaseCharacter* /*Character*/)
{
	// 기본은 아무 동작 없음 ? 파생 클래스에서 구현
}