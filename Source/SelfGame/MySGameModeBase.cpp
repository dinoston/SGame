#include "MySGameModeBase.h"
#include "Character/BaseCharacter.h"
#include "Player/MyPlayerController.h"
#include "MyHUD.h"
#include "MyGameStateBase.h"
#include "MyPlayerState.h"
#include "GameFramework/SpectatorPawn.h" // 기본 관전 폰 사용 시

AMySGameModeBase::AMySGameModeBase()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	HUDClass = AMyHUD::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	SpectatorClass = ASpectatorPawn::StaticClass();

}

//static ConstructorHelpers::FClassFinder<AHUD> HUD_BP(TEXT("/Game/UI/BP_MyHUD.BP_MyHUD_C"));
//if (HUD_BP.Succeeded()) HUDClass = HUD_BP.Class;