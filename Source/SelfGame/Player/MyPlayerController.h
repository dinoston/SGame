
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;


UCLASS()
class SELFGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	virtual void BeginPlay() override;

	/** 적용할 IMC (에디터에서 IMC_Player 지정) */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultIMC = nullptr;

	/** IMC 우선순위(보통 0) */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	int32 IMC_Priority = 0;
	

};
