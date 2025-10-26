// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "../Player/MyPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"   // ★ 이거 추가


void UMainMenuWidget::OnClickStart()
{
    if (auto* PC = Cast<AMyPlayerController>(GetOwningPlayer()))
    {
        PC->StartGame();
    }
}

void UMainMenuWidget::OnClickOption() { /* 옵션 패널 열기 */ }


void UMainMenuWidget::OnClickQuit()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
