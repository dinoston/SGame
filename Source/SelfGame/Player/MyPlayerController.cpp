

#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../HealthComponent.h"     // ★ 추가
#include "GameFramework/PlayerController.h"

#include "../UI/WBP_UI.h" // UWBP_UI로 캐스팅해 InitFromPawn 호출하려면

#include "../Character/BaseCharacter.h"

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

	// 처음엔 메인 메뉴
	ShowMainMenu();

	//bShowMouseCursor = false;          // 커서 숨김
	//FInputModeGameOnly Mode;           // 게임 입력만
	//SetInputMode(Mode);

	// ★ 피치(상하) 한계 설정
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -30.f;  // 아래로 최대 
		PlayerCameraManager->ViewPitchMax = 30.f;  // 위로 최대 

		//PlayerCameraManager->ViewYawMin = -90.f;   // 왼쪽 최대
		//PlayerCameraManager->ViewYawMax = 90.f;   // 오른쪽 최대
	}
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BindToPawn(InPawn);               // 리스폰/포제션 때 사망 이벤트 재연결
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
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Attack"));
	if (ABaseCharacter* C = GetPawn<ABaseCharacter>())
	{
		C->FirePressed(); // ★ 실제 발사
	}

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


// ================= UI 전환 =================
void AMyPlayerController::ShowMainMenu()
{
	SetPause(true);
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly{});

	if (!MainMenu && MainMenuClass) 
		MainMenu = CreateWidget<UUserWidget>(this, MainMenuClass);
	if (MainMenu && !MainMenu->IsInViewport()) 
		MainMenu->AddToViewport(100);

	if (InGameUI) 
		InGameUI->RemoveFromParent();
	if (EndGameUI) 
		EndGameUI->RemoveFromParent();

	StopCountdown();   // 메뉴로 가면 카운트다운 중지

}

void AMyPlayerController::ShowInGameUI()
{
	SetPause(false);
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly{});

	if (!InGameUI && InGameUIClass) 
		InGameUI = CreateWidget<UUserWidget>(this, InGameUIClass);
	if (InGameUI && !InGameUI->IsInViewport()) 
		InGameUI->AddToViewport(10);

	if (MainMenu) 
		MainMenu->RemoveFromParent();
	if (EndGameUI) 
		EndGameUI->RemoveFromParent();

	// UWBP_UI라면 현재 Pawn으로 초기화
	if (auto* UI = Cast<UWBP_UI>(InGameUI))
		if (APawn* P = GetPawn())
			UI->InitFromPawn(P);

	// ★ 카운트다운 시작
	StartCountdown();
}

void AMyPlayerController::ShowEndGameUI()
{
	SetPause(true);
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly{});

	if (!EndGameUI && EndGameClass) 
		EndGameUI = CreateWidget<UUserWidget>(this, EndGameClass);
	if (EndGameUI && !EndGameUI->IsInViewport()) 
		EndGameUI->AddToViewport(200);

	if (InGameUI) 
		InGameUI->RemoveFromParent();
	if (MainMenu) 
		MainMenu->RemoveFromParent();

	StopCountdown();   // 엔드게임 화면에서 중지

}

// 메뉴 버튼에서 불릴 공개 함수들
void AMyPlayerController::StartGame()
{
	ShowInGameUI();
}
void AMyPlayerController::GoToMainMenu()
{
	ShowMainMenu();
}
void AMyPlayerController::RestartCurrentLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

// ================= 사망 감지 =================
void AMyPlayerController::BindToPawn(APawn* P)
{
	UnbindFromPawn();
	if (!P) return;

	if (UHealthComponent* HC = P->FindComponentByClass<UHealthComponent>())
	{
		HC->OnDeath.AddDynamic(this, &AMyPlayerController::OnPawnDied);
		BoundHealth = HC;
	}

	// 인게임 UI가 이미 떠 있었으면 새 Pawn으로 재초기화
	if (auto* UI = Cast<UWBP_UI>(InGameUI))
		UI->InitFromPawn(P);
}

void AMyPlayerController::UnbindFromPawn()
{
	if (BoundHealth)
	{
		BoundHealth->OnDeath.RemoveDynamic(this, &AMyPlayerController::OnPawnDied);
		BoundHealth = nullptr;
	}
}

void AMyPlayerController::OnPawnDied()
{
	ShowEndGameUI();
}

// ------------ Countdown 구현 ------------
void AMyPlayerController::StartCountdown()
{
	CountdownRemaining = CountdownDuration;

	// UI에 초기값 뿌리기
	if (auto* UI = Cast<UWBP_UI>(InGameUI))
		UI->UpdateCountdown(CountdownRemaining);

	// 0.1초 간격으로 갱신 (부드럽게 보이고 너무 잦지도 않게)
	GetWorldTimerManager().SetTimer(
		Timer_Countdown, this, &AMyPlayerController::TickCountdown, 0.1f, true
	);
}

void AMyPlayerController::StopCountdown()
{
	GetWorldTimerManager().ClearTimer(Timer_Countdown);
}

void AMyPlayerController::TickCountdown()
{
	CountdownRemaining = FMath::Max(0.f, CountdownRemaining - 0.1f);

	if (auto* UI = Cast<UWBP_UI>(InGameUI))
		UI->UpdateCountdown(CountdownRemaining);

	if (CountdownRemaining <= 0.f)
	{
		// 끝! 카운트다운 중지하고 즉시 사망 처리
		StopCountdown();

		if (APawn* P = GetPawn())
			if (auto* HC = P->FindComponentByClass<UHealthComponent>())
				HC->TakeDamage(99999.f);  // 큰 수로 즉사

		// 사망하면 OnPawnDied() -> ShowEndGameUI()로 넘어감
	}
}