// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EscapeRoomGameModeBase.h"
#include "Utils/Definitions.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API ALobbyGameMode : public AEscapeRoomGameModeBase
{
	GENERATED_BODY()


public:
	void ChangeCharacterStatus(ECharacterType Character);

	void StartGame();

protected:

	// Called after a successful login This is the first place it is safe to call replicated functions on the PlayerController
	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

	void UpdateAllCharactersLobbyMenu();

	bool AllPlayersReady();

	

private:

	// List of player connected after they log in (postlogin)
	TArray<class ALobbyPlayerController*> PlayerConnectedList;

	UPROPERTY(EditDefaultsOnly, Category = "Server Settings")
	TArray<FCharacterType> AvalaibleCharacters;
	
};
