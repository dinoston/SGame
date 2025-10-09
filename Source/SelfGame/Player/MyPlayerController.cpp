

#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
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
		PlayerCameraManager->ViewPitchMin = -30.f;  // 아래로 최대 
		PlayerCameraManager->ViewPitchMax = 30.f;  // 위로 최대 

		//PlayerCameraManager->ViewYawMin = -90.f;   // 왼쪽 최대
		//PlayerCameraManager->ViewYawMax = 90.f;   // 오른쪽 최대
	}
}


void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if(IA_Attack)	EIC->BindAction(IA_Attack, ETriggerEvent::Started, this, &ThisClass::Input_Attack);
		if (IA_Move)	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		if (IA_Look)	EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
		
		// ★ 점프: 누를 때/뗄 때
		if (IA_Jump)
		{

			EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ThisClass::Input_JumpStart); // 눌렀을 때
			//EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ThisClass::Input_JumpStop);  // 뗐을 때
			//EIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ThisClass::Input_JumpStop);  // 취소

		}
	}
}

void AMyPlayerController::Input_Attack(const FInputActionValue& InputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Attack"));

}

void AMyPlayerController::Input_Move(const FInputActionValue& InputValue)
{
	const FVector2D Axis = InputValue.Get<FVector2D>();
	APawn* P = GetPawn();
	if (!P) return;

	// 컨트롤러(Yaw) = 카메라 기준
	const FRotator YawRot(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Fwd = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y); // 정면
	const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X); // 오른쪽

	// IMC에서 W/S→Y, A/D→X 로 매핑했을 때:
	if (Axis.Y != 0.f) P->AddMovementInput(Fwd, Axis.Y);  // W=앞, S=뒤
	if (Axis.X != 0.f) P->AddMovementInput(Right, Axis.X);  // D=오른쪽, A=왼쪽
}

void AMyPlayerController::Input_Look(const FInputActionValue& InputValue)
{
	const FVector2D Look = InputValue.Get<FVector2D>();
	AddYawInput(Look.X);
	AddPitchInput(Look.Y * -1.f);

}

void AMyPlayerController::Input_JumpStart(const FInputActionValue&)
{
	if (ACharacter* C = GetPawn<ACharacter>()) C->Jump();
}
//void AMyPlayerController::Input_JumpStop(const FInputActionValue&)
//{
//	if (ACharacter* C = GetPawn<ACharacter>()) C->StopJumping();
//}


