// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Utils/Definitions.h"

#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALobbyPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void Client_Initialize();

	UFUNCTION(Client, Reliable)
	void Client_RemoveMenu();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SelectCharacter(ECharacterType NewCharacter);

	UFUNCTION(Client, Reliable)
	void Client_UpdateAvailableCharacters(const TArray<FCharacterType>& InAvalaibleCharacters);

	UFUNCTION(Client, Reliable)
	void Client_UpdateReadyButton();

	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerController")
	void OnRoomCompleted(APawn* InstigatorPawn, bool bSuccess);

	// GAMEPLAY ROOM IMPLEMENTATION
	UFUNCTION(Client, Reliable)
	void Client_InitializeRoom(TSubclassOf<APawn> PawnToSpawn);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_CreateInGameUI();

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_UpdateInGameUI(const FString& Text);

	UFUNCTION(BLueprintCallable, Category = "UI")
	void UpdateInGameUI(const FString& Text);

	// GAMEPLAY ROOM IMPLEMENTATION

	FORCEINLINE class UInGamePlayer* GetPlayerUI() const { return InGameUI; }

	

protected:
	UPROPERTY(BlueprintReadWrite, Category = "UI Player")
	class UInGamePlayer* InGameUI;

private:
	TSubclassOf<class UUserWidget> LobbyMenuClass;
	class ULobbyMenu* LobbyMenu;

	UPROPERTY(Replicated)
	TArray<FCharacterType> AvalaibleCharacters;	

	TSubclassOf<class UUserWidget> InGameUIClass;
	
};
