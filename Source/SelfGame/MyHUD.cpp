// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
    // 예: 좌상단에 간단 텍스트
    if (Canvas)
    {
        DrawText(TEXT("MyHUD"), FLinearColor::White, 20.f, 20.f, nullptr, 1.2f);
    }
}
