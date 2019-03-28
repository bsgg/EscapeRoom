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

	// GAMEPLAY INTERACTIVE INTERFACES
	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void ShowDebugLog(const FString& Text);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void ShowMessage(const FString& Text);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void HideMessage();

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void ShowControls();

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void HideControls();
	// GAMEPLAY ROOM IMPLEMENTATION








	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_UpdateInGameMessageUI(const FString& Text, bool hideMessages = false);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_UpdateInGame(const AInteractiveBase* Interactive);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_HideInGameMessage();


	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_UpdateControlsUI(const AInteractiveBase* Interactive);
	// GAMEPLAY ROOM IMPLEMENTATION




	// INVENTORY FUNCTIONS
	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_ToggleInventory();	

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_NavigateInventory(EDirectionType Direction);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_AddObjectToSlot(const FObjectInteraction& Object);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_RemoveObjectFromSlot(const FName& ObjectID);
	
	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_OnSelectItemInInventory(const FObjectInteraction& SelectedObject);

	// INVENTORY FUNCTIONS


	FObjectInteraction* FindCombinedObject(FName ObjectID_A, FName ObjectID_B) const;


protected:
	UPROPERTY(BlueprintReadWrite, Category = "UI Player")
	class UInGamePlayer* InGameUI;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FName SelectedObjectID;
	// TODO CREATE PAUSE MENU


	virtual void SetupInputComponent() override;

private:
	TSubclassOf<class UUserWidget> LobbyMenuClass;
	class ULobbyMenu* LobbyMenu;

	UPROPERTY(Replicated)
	TArray<FCharacterType> AvalaibleCharacters;	

	TSubclassOf<class UUserWidget> InGameUIClass;	

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ObjectDB; // Objects database database

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ObjectCombinationDB; // Objects database database


	FTimerHandle InteractionTimerHandle;

	void EndInteraction();




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

public:

	void AddItemToInventory(const FName& ObjID);

	
};
