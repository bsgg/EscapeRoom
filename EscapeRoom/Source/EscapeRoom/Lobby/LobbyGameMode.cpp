// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "LobbyPlayerController.h"
#include "Game/EscapeRoomPlayerState.h"
#include "EscapeRoomGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyGameMode::PostLogin]"));

	// Initialize Player Controller (create its own menu)
	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(NewPlayer);
	if (LobbyPC == nullptr) return;

	LobbyPC->Client_Initialize();

	PlayerConnectedList.Add(LobbyPC);

	UpdateAllCharactersLobbyMenu();

}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}


void ALobbyGameMode::ChangeCharacterStatus(ECharacterType Character)
{
	FString Char = GetEnumValueAsString<ECharacterType>("ECharacterType", Character);

	//UE_LOG(LogTemp, Warning, TEXT("[MakeCharacterUnavaliable] %s"), *Char);

	uint8 IndexCurrent = (uint8)Character;

	if (IndexCurrent < AvalaibleCharacters.Num())
	{
		if (AvalaibleCharacters[IndexCurrent].bIsAvailable) // If that character was avaliable, set to false
		{
			//UE_LOG(LogTemp, Warning, TEXT("[MakeCharacterUnavaliable] Char = %s is available"), *Char);

			AvalaibleCharacters[IndexCurrent].bIsAvailable = false;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("[MakeCharacterUnavaliable] Char = %s is not available"), *Char);

			AvalaibleCharacters[IndexCurrent].bIsAvailable = true;
		}

		UpdateAllCharactersLobbyMenu();

		// Check if all players are ready
		if (AllPlayersReady())
		{
			for (ALobbyPlayerController* PC : PlayerConnectedList)
			{
				PC->Client_UpdateReadyButton();
			}
		}
	}
}

bool ALobbyGameMode::AllPlayersReady()
{
	// At least 2 players
	if (PlayerConnectedList.Num() < 2) return false;

	for (ALobbyPlayerController* PC : PlayerConnectedList)
	{
		if ((PC == nullptr) || (PC->PlayerState == nullptr)) return false;

		AEscapeRoomPlayerState* GamePlayerState = Cast<AEscapeRoomPlayerState>(PC->PlayerState);

		if (GamePlayerState == nullptr) return false;
		
		if (GamePlayerState->SelectedCharacter == ECharacterType::VE_NONE) return false;

	}

	return true;
}

void ALobbyGameMode::UpdateAllCharactersLobbyMenu()
{
	//UE_LOG(LogTemp, Warning, TEXT("[LobbyGameMode::UpdateMenuClients] PConnected: %i "), PlayerConnectedList.Num());

	for (ALobbyPlayerController* PC : PlayerConnectedList)
	{
		PC->Client_UpdateAvailableCharacters(AvalaibleCharacters);
	}
}

void ALobbyGameMode::StartGame()
{
	UEscapeRoomGameInstance* GameInstance = Cast<UEscapeRoomGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	// Remove menus from players
	for (ALobbyPlayerController* PC : PlayerConnectedList)
	{
		PC->Client_RemoveMenu();

		AEscapeRoomPlayerState* PlayerState = Cast<AEscapeRoomPlayerState>(PC->PlayerState);
		if (PlayerState != nullptr)
		{
			GameInstance->AddPersistentPlayerState(PlayerState);
		}
	}

	
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	bUseSeamlessTravel = true;
	UE_LOG(LogTemp, Warning, TEXT("[ALobbyGameMode::Server Travel to game]"));
	World->ServerTravel("/Game/Maps/Room1?listen");
}