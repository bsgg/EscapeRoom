// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoomGameState.h"
#include "Lobby/LobbyPlayerController.h"

void AEscapeRoomGameState::MulticastOnRoomCompleted_Implementation(APawn* InstigatorPawn, bool bSuccess)
{
	// Loop through all the player controllers available
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(It->Get());
		// Check when we are the owner of the controller
		if (PC && PC->IsLocalController())
		{			
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeRoomGameState::MulticastOnRoomCompleted] Disabled Input on %s"), *PC->GetName());

			PC->OnRoomCompleted(InstigatorPawn, bSuccess);

			// Disable input
			APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				Pawn->DisableInput(PC);
			}
		}
	}
}