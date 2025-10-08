#include "MyGameStateBase.h"
#include "Net/UnrealNetwork.h"

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const
{
    Super::GetLifetimeReplicatedProps(Out);
    //DOREPLIFETIME(AMyGameState, Round);
}