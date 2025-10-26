
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"   // 추가

#include "MyPlayerController.generated.h"


struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SELFGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	AMyPlayerController(const FObjectInitializer& ObjectInitializer);

	// 메뉴에서 버튼이 호출할 함수들
	UFUNCTION(BlueprintCallable) void StartGame();     // Start 클릭
	UFUNCTION(BlueprintCallable) void GoToMainMenu();  // Main Menu 클릭(엔드게임 등)
	UFUNCTION(BlueprintCallable) void RestartCurrentLevel();  // Restart 클릭(엔드게임)

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Input_Attack(const FInputActionValue& InputValue);
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Look(const FInputActionValue& InputValue);

	// ★ 점프: 누름/뗌을 분리
	void Input_JumpStart(const FInputActionValue& InputValue);
	//void Input_JumpStop(const FInputActionValue& InputValue);

	 // ---------- UI 전환 내부함수 ----------
	void ShowMainMenu();
	void ShowInGameUI();
	void ShowEndGameUI();

	// ---------- 사망 감지 ----------
	virtual void OnPossess(APawn* InPawn) override;
	void BindToPawn(APawn* P);
	void UnbindFromPawn();
	UFUNCTION() void OnPawnDied();   // HealthComponent의 OnDeath 콜백

	// ---------- 위젯 클래스 지정 & 인스턴스 ----------
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuClass;   // WBP_StartGame
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InGameUIClass;   // WBP_UI (당신 HUD 위젯)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> EndGameClass;    // WBP_EndGame

	UPROPERTY() TObjectPtr<UUserWidget> MainMenu = nullptr;
	UPROPERTY() TObjectPtr<UUserWidget> InGameUI = nullptr;
	UPROPERTY() TObjectPtr<UUserWidget> EndGameUI = nullptr;

	UPROPERTY() class UHealthComponent* BoundHealth = nullptr;

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;
	

};
