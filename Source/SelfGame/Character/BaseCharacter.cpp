#include "BaseCharacter.h"
#include "../HealthComponent.h"

ABaseCharacter::ABaseCharacter()
{
    HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    // PrimaryActorTick.bCanEverTick = true; // �ʿ��ϸ� �Ѽ���
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}