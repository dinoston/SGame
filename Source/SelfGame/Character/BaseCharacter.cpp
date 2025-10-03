#include "BaseCharacter.h"
#include "../HealthComponent.h"

ABaseCharacter::ABaseCharacter()
{
    HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    // PrimaryActorTick.bCanEverTick = true; // 필요하면 켜세요
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}