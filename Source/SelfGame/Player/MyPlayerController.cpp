

#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LP = GetLocalPlayer())
		if (auto* Subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
			if (DefaultIMC) Subsys->AddMappingContext(DefaultIMC, 0);

	bShowMouseCursor = false;          // 커서 숨김
	FInputModeGameOnly Mode;           // 게임 입력만
	SetInputMode(Mode);

	// ★ 피치(상하) 한계 설정
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -45.f;  // 아래로 최대 60도
		PlayerCameraManager->ViewPitchMax = 45.f;  // 위로 최대 45도

		PlayerCameraManager->ViewYawMin = -90.f;   // 왼쪽 최대
		PlayerCameraManager->ViewYawMax = 90.f;   // 오른쪽 최대
	}
}


void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &ThisClass::Input_Attack);
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
		EIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ThisClass::Input_Jump);
	}
}

void AMyPlayerController::Input_Attack(const FInputActionValue& InputValue)
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Cyan, TEXT("Attack"));

}

void AMyPlayerController::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	if (MovementVector.X != 0)
	{
		FVector Direction = FVector::ForwardVector * MovementVector.X;
		GetPawn()->AddMovementInput(Direction * 50.f);
	}
	if (MovementVector.Y != 0)
	{
		FVector Direction = FVector::RightVector * MovementVector.Y;
		GetPawn()->AddMovementInput(Direction * 50.f);
	}
}

void AMyPlayerController::Input_Look(const FInputActionValue& InputValue)
{
	const FVector2D Look = InputValue.Get<FVector2D>();
	AddYawInput(Look.X);
	AddPitchInput(Look.Y);

}

void AMyPlayerController::Input_Jump(const FInputActionValue& InputValue)
{

}
