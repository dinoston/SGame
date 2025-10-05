// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력 모드/마우스 커서
	bShowMouseCursor = false;
	FInputModeGameOnly Mode; SetInputMode(Mode);

	// 로컬 플레이어 서브시스템에 IMC 적용
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsys =
			LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultIMC)
			{
				Subsys->AddMappingContext(DefaultIMC, IMC_Priority);
			}
		}
	}
}