// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void AMyHUD::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = GetOwningPlayerController())
    {
        if (RootUIClass)
        {
            RootUI = CreateWidget<UUserWidget>(PC, RootUIClass);
            if (RootUI)
            {
                RootUI->AddToViewport();

              
            }
        }
    }
}