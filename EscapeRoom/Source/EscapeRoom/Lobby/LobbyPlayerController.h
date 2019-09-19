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

	virtual void Tick(float DeltaTime) override;

public:

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
	void ShowMessage(const FString& Text, float time);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void HideMessage();

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void ShowControls();

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void HideControls();


	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void CreateInteractiveUI(const FName& WidgetID);

	UFUNCTION(Client, Reliable, BLueprintCallable, Category = "UI")
	void RemoveInteractiveUI();

	FORCEINLINE class UUIBasicInteractive* GetInteractiveUI() const { return InteractiveUI; };


	///////////// GAMEPLAY UI IMPLEMENTATION ////////////////

	///////////// PAUSE UI IMPLEMENTATION ////////////////
	void UnPause();
	///////////// PAUSE UI IMPLEMENTATION ////////////////

protected:
	void NavigateInventoryRight();
	void NavigateInventoryLeft();
	void ToggleInventory();

	void TogglePauseMenu();
public:

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

	TSubclassOf<class UUserWidget> PauseMenuClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI Player")
	class UPauseMenu* PauseMenuUI;
	bool bPauseGame = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;


	

protected:

///////////// Character input////////////////
	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void MoveRight(float Value);	


	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void FaceButtonBottom();

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void FaceButtonRight();

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void FaceButtonTop();

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void FaceButtonLeft();


	//UFUNCTION(BlueprintCallable, Category = "Input Actions")
	//void ExitInteractiveUI();

	AMainCharacter* myCharacter;


	bool bIsInteractiveUIOpened = false;

	class UUIBasicInteractive* InteractiveUI;

	bool lockNavigateInteractiveUI = false;

	float lockNavigateInteractiveUIElapsed = 0.0f;
///////////// Character input////////////////

};
