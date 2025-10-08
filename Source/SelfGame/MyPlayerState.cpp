#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const
{
    Super::GetLifetimeReplicatedProps(Out);
    //DOREPLIFETIME(AMyPlayerState, Health);
}