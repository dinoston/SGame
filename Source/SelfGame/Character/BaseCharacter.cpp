#include "BaseCharacter.h"
#include "../HealthComponent.h"
#include "InputCoreTypes.h"
#include "Engine/Engine.h"

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

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindKey(EKeys::Zero, IE_Pressed, this, &ABaseCharacter::TestTakeDamage);

}

void ABaseCharacter::TestTakeDamage()
{
    if (!HealthComp) return;

    HealthComp->TakeDamage(50.f);

    // (선택) 화면에 현재 HP 표시 — 안 써도 됨
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("HP: %.1f"), HealthComp->CurrentHP));
    }

}
