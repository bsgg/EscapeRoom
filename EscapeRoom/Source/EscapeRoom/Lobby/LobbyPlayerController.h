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

	///////////// LOBBY IMPLEMENTATION ////////////////
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
	///////////// LOBBY IMPLEMENTATION ////////////////
	

	///////////// GAMEPLAY ROOM IMPLEMENTATION ////////////////
	UFUNCTION(Client, Reliable)
	void Client_InitializeRoom(TSubclassOf<APawn> PawnToSpawn);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_CreateInGameUI();
	///////////// GAMEPLAY ROOM IMPLEMENTATION ////////////////

	///////////// GAMEPLAY UI IMPLEMENTATION ////////////////
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
	///////////// GAMEPLAY UI IMPLEMENTATION ////////////////







	/*
	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_UpdateInGameMessageUI(const FString& Text, bool hideMessages = false);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_UpdateInGame(const AInteractiveBase* Interactive);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_HideInGameMessage();


	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_UpdateControlsUI(const AInteractiveBase* Interactive);

	*/



	///////////// GAMEPLAY INVENTORY IMPLEMENTATION ////////////////
protected:
	void NavigateInventoryRight();
	void NavigateInventoryLeft();
	void ToggleInventory();


public:
	//UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	//void Client_ToggleInventory();	

	//UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	//void Client_NavigateInventory(EDirectionType Direction);

	

	//UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	//void Client_AddObjectToSlot(const FObjectInteraction& Object);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void Client_RemoveObjectFromSlot(const FName& ObjectID);
	
	//UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	//void Client_OnSelectItemInInventory(const FObjectInteraction& SelectedObject);

	void AddItemToInventory(const FName& ObjID);

	void RemoveItemFromInventory(const FName& ObjID);

	FName GetSelectedItem();

	///////////// GAMEPLAY INVENTORY IMPLEMENTATION ////////////////


	///////////// OLD EXAMPLE EVENT WHEN ROOM IS COMPLETED////////////////
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerController")
	void OnRoomCompleted(APawn* InstigatorPawn, bool bSuccess);
	///////////// OLD ////////////////

protected:
	
	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FName SelectedObjectID;

	UPROPERTY(Replicated)
	TArray<FCharacterType> AvalaibleCharacters;

	TSubclassOf<class UUserWidget> LobbyMenuClass;
	class ULobbyMenu* LobbyMenu;

	TSubclassOf<class UUserWidget> InGameUIClass;	

	UPROPERTY(BlueprintReadWrite, Category = "UI Player")
	class UInGamePlayer* InGameUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;


	
};
