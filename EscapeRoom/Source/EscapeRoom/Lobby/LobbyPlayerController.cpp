// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

#include "LobbyMenu.h"
#include "LobbyGameMode.h"
#include "Game/EscapeRoomPlayerState.h"
#include "Characters/MainCharacter.h"

#include "Game/Components/InventoryComponent.h"
#include "Game/GameLogic/Objects/ObjectHelper.h"

#include "Game/RoomGameMode.h"
#include "Game/UI/InGamePlayer.h"
#include "Game/UI/InventoryUI.h"

#include "Utils/MenuWidget.h"
#include "Game/UI/PauseMenu.h"
#include "EscapeRoomGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"

#include "UnrealNetwork.h"

ALobbyPlayerController::ALobbyPlayerController() 
{
	// Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyMenuBPClass(TEXT("/Game/Lobby/LobbyMenu_WBP"));

	if (LobbyMenuBPClass.Class != nullptr)
	{
		LobbyMenuClass = LobbyMenuBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/Game/UI/InGamePlayer_WBP"));

	if (InGameMenuBPClass.Class != nullptr)
	{
		InGameUIClass = InGameMenuBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/Game/UI/WBP_PauseMenu"));

	if (PauseMenuBPClass.Class != nullptr)
	{
		PauseMenuClass = PauseMenuBPClass.Class;
	}

}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerState == nullptr) return;

	AEscapeRoomPlayerState* ESPlayerState = Cast<AEscapeRoomPlayerState>(PlayerState);
	if (ESPlayerState == nullptr) return;

	FString Authority = "";

	if (HasAuthority())
	{
		Authority = " -SERVER- ";
	}
	else
	{
		Authority = " -CLIENT- ";
	}

	FString TypeChar = GetEnumValueAsString<ECharacterType>("ECharacterType", ESPlayerState->SelectedCharacter);

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::BeginPlay] %s = ChararcerSelected %s"), *Authority, *TypeChar);

	bPauseGame = false;

	APawn* const pawn = GetPawn();

	if (pawn != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::BeginPlay] APawn not null"));
		myCharacter = Cast<AMainCharacter>(pawn);
	}

}

void ALobbyPlayerController::Tick(float DeltaTime)
{
	if (lockNavigateInteractiveUI)
	{
		lockNavigateInteractiveUIElapsed += DeltaTime;

		if (lockNavigateInteractiveUIElapsed >= 0.3f)
		{
			lockNavigateInteractiveUI = false;
		}
	}
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


	// Bind inventory
	InputComponent->BindAction("NavigateInventoryLeft", IE_Pressed, this, &ALobbyPlayerController::NavigateInventoryLeft);
	InputComponent->BindAction("NavigateInventoryRight", IE_Pressed, this, &ALobbyPlayerController::NavigateInventoryRight);
	InputComponent->BindAction("InputToggleInventory", IE_Pressed, this, &ALobbyPlayerController::ToggleInventory);

	InputComponent->BindAction("Pause", IE_Pressed, this, &ALobbyPlayerController::TogglePauseMenu);

	InputComponent->BindAxis("MoveForward", this, &ALobbyPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALobbyPlayerController::MoveRight);

	InputComponent->BindAction("Inspect", IE_Pressed, this, &ALobbyPlayerController::Inspect);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ALobbyPlayerController::Interact);
}


///////////// LOBBY IMPLEMENTATION ////////////////
void ALobbyPlayerController::Client_Initialize_Implementation()
{

	// Create menu lobby
	if (LobbyMenuClass == nullptr) return;

	LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyMenuClass);

	if (LobbyMenu == nullptr) return;

	LobbyMenu->Setup();

}

void ALobbyPlayerController::Client_RemoveMenu_Implementation()
{
	if (LobbyMenu == nullptr) return;

	LobbyMenu->Teardown();
}

void ALobbyPlayerController::Server_SelectCharacter_Implementation(ECharacterType NewCharacter)
{
	ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr) return;

	if (PlayerState == nullptr) return;

	AEscapeRoomPlayerState* GamePlayerState = Cast<AEscapeRoomPlayerState>(PlayerState);
	if (GamePlayerState != nullptr)
	{
		FString SelectedChar = GetEnumValueAsString< ECharacterType>("ECharacterType", GamePlayerState->SelectedCharacter);
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Server_SelectCharacter]SelectedChar: %s "), *SelectedChar);

		if (GamePlayerState->SelectedCharacter == ECharacterType::VE_NONE)
		{
			GamePlayerState->SelectedCharacter = NewCharacter;
			GM->ChangeCharacterStatus(GamePlayerState->SelectedCharacter);
		}
		else
		{
			GM->ChangeCharacterStatus(GamePlayerState->SelectedCharacter);
			GamePlayerState->SelectedCharacter = NewCharacter;
			GM->ChangeCharacterStatus(NewCharacter);
		}
	}
}

bool ALobbyPlayerController::Server_SelectCharacter_Validate(ECharacterType NewCharacter)
{
	return true;
}


void ALobbyPlayerController::Client_UpdateAvailableCharacters_Implementation(const TArray<FCharacterType>& InAvalaibleCharacters)
{
	AvalaibleCharacters = InAvalaibleCharacters;

	UE_LOG(LogTemp, Warning, TEXT("[Client_UpdateAvailableCharacters] Num AvalaibleCharacters %i "), AvalaibleCharacters.Num());

	if (LobbyMenu == nullptr) return;

	// Update Available character on client
	for (FCharacterType CharT : AvalaibleCharacters)
	{
		// Disable or Update the character
		if (CharT.bIsAvailable)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Client_UpdateAvailableCharacters] CharT %s = Available "), *CharT.CharacterName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[Client_UpdateAvailableCharacters] CharT %s = Not Available "), *CharT.CharacterName.ToString());
		}

		LobbyMenu->SetActiveCharacterButton(CharT.CharacterType, CharT.bIsAvailable);
	}
}

void ALobbyPlayerController::Client_UpdateReadyButton_Implementation()
{
	if (LobbyMenu == nullptr) return;

	LobbyMenu->UpdateReadyState();
}

///////////// LOBBY IMPLEMENTATION ////////////////

///////////// GAMEPLAY ROOM IMPLEMENTATION ////////////////
void ALobbyPlayerController::Client_InitializeRoom_Implementation(TSubclassOf<APawn> PawnToSpawn)
{   
	// TODO: SETUP IN GAME
	if (this->PlayerState == nullptr) return;

	FString Authority = "";

	if (HasAuthority())
	{
		Authority = " -SERVER- ";
	}
	else
	{
		Authority = " -CLIENT- ";
	}

	AEscapeRoomPlayerState* ESPlayerScape = Cast<AEscapeRoomPlayerState>(this->PlayerState);
	if (ESPlayerScape != nullptr)
	{
		FString TCharacter = GetEnumValueAsString<ECharacterType>("ECharacterType", ESPlayerScape->SelectedCharacter);
		FString Message = Authority + " " + TCharacter;
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_InitializeRoom] %s  - MyCharacter: %s"), *Authority, *TCharacter);

		if (InGameUI != nullptr)
		{
			InGameUI->SetPortrait(ESPlayerScape->SelectedCharacter);
		}
	}
}

void ALobbyPlayerController::Client_CreateInGameUI_Implementation()
{
	// Create UI
	if (InGameUIClass == nullptr) return;
	InGameUI = CreateWidget<UInGamePlayer>(this, InGameUIClass);

	if (InGameUI == nullptr) return;
	InGameUI->AddToViewport();
	
	// Create Pause UI
	if (PauseMenuClass == nullptr) return;	
	PauseMenuUI = CreateWidget<UPauseMenu>(this, PauseMenuClass);

	if (PauseMenuUI == nullptr) return;
	UEscapeRoomGameInstance* GameInstance = Cast<UEscapeRoomGameInstance>(GetGameInstance());
	if (GameInstance != nullptr)
	{
		PauseMenuUI->SetMenuInteraface(GameInstance);
	}

}
///////////// GAMEPLAY ROOM IMPLEMENTATION ////////////////


///////////// GAMEPLAY UI IMPLEMENTATION ////////////////
void  ALobbyPlayerController::ShowDebugLog_Implementation(const FString& Text)
{
	if (InGameUI == nullptr) return;

	InGameUI->DebugLog(Text);
}

void ALobbyPlayerController::ShowMessage_Implementation(const FString& Text, float time)
{
	if (InGameUI == nullptr) return;
	InGameUI->ShowMessage(Text, time);
}

void ALobbyPlayerController::HideMessage_Implementation()
{
	if (InGameUI == nullptr) return;
	//InGameUI->HideMessage();
}


void ALobbyPlayerController::ShowControls_Implementation()
{
	if (InGameUI == nullptr) return;

	InGameUI->ShowControls(true, true, true);
}

void ALobbyPlayerController::HideControls_Implementation()
{
	if (InGameUI == nullptr) return;

	InGameUI->HideControls();
}


void ALobbyPlayerController::CreateInteractiveUI_Implementation(const FName& WidgetID)
{
	FInteractiveUI* WidgetData = UObjectHelper::GetWidgetByID(WidgetID);
	if ((WidgetData != nullptr) && (WidgetData->UI != nullptr))

	InteractiveUI = CreateWidget<UUIBasicInteractive>(this, WidgetData->UI);

	if (InteractiveUI != nullptr)
	{
		InteractiveUI->InitializeWidget(WidgetData->ExtraInformation);

		InteractiveUI->AddToViewport();

		InteractiveUI->OnShowWidget();


		bIsInteractiveUIOpened = true;

		/*FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(InteractiveUI->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		SetInputMode(InputModeData);*/
		//bShowMouseCursor = true;		
	}
	
}

void ALobbyPlayerController::RemoveInteractiveUI_Implementation()
{
	if (InteractiveUI != nullptr)
	{
		InteractiveUI->RemoveFromViewport();

		/*FInputModeGameOnly InputModeData;
		SetInputMode(InputModeData);

		//bShowMouseCursor = false;

		bIsInteractiveUIOpened = false;*/
	}

}

///////////// GAMEPLAY UI IMPLEMENTATION ////////////////


///////////// GAMEPLAY INVENTORY IMPLEMENTATION ////////////////
void ALobbyPlayerController::NavigateInventoryLeft()
{
	if (InGameUI == nullptr) return;
	InGameUI->NavigateInventory(EDirectionType::VE_LEFT);
}

void ALobbyPlayerController::NavigateInventoryRight()
{
	if (InGameUI == nullptr) return;

	InGameUI->NavigateInventory(EDirectionType::VE_RIGHT);
}

void ALobbyPlayerController::ToggleInventory()
{
	if (InGameUI == nullptr) return;

	InGameUI->ToggleInventory();
}

FName ALobbyPlayerController::GetSelectedItem()
{
	if (InGameUI == nullptr) return "NONE";
	
	return InGameUI->GetSelectedItem();
}


void ALobbyPlayerController::AddItemToInventory(const FName& ObjID)
{
	if (InventoryComponent == nullptr) return;

	FObjectInteraction* NewObject = UObjectHelper::GetObjectByID(ObjID);
	
	if (NewObject == nullptr) return;

	InventoryComponent->AddObject(ObjID, *NewObject);

	if (InGameUI == nullptr) return;

	InGameUI->AddObjectToSlot(*NewObject);
}

void ALobbyPlayerController::RemoveItemFromInventory(const FName& ObjID)
{
	if (InventoryComponent == nullptr) return;

	InventoryComponent->RemoveObject(ObjID);

	if (InGameUI == nullptr) return;

	InGameUI->RemoveObjectFromSlot(ObjID);
}
///////////// GAMEPLAY INVENTORY IMPLEMENTATION ////////////////


///////////// PAUSE MENU IMPLEMENTATION ////////////////

void ALobbyPlayerController::TogglePauseMenu()
{
	if (PauseMenuUI == nullptr) return;

	if (bPauseGame)
	{
		bPauseGame = false;
		PauseMenuUI->Teardown();
	}
	else
	{
		bPauseGame = true;
		PauseMenuUI->Setup();
	}
}

void ALobbyPlayerController::UnPause()
{
	if (PauseMenuUI == nullptr) return;
	bPauseGame = false;
	PauseMenuUI->Teardown();
}

///////////// PAUSE MENU IMPLEMENTATION ////////////////



///////////// CHARACTER INPUT ////////////////

void ALobbyPlayerController::MoveRight(float Value)
{
	if (Value == 0.0f) return;

	if (bIsInteractiveUIOpened)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::MoveRight] bIsInteractiveUIOpened %f"), Value);

		if (InteractiveUI != nullptr)
		{
			if (lockNavigateInteractiveUI) return;

			if (Value >= 0.2f)
			{
				lockNavigateInteractiveUI = true;
				lockNavigateInteractiveUIElapsed = 0.0f;
				InteractiveUI->Navigate(EDirectionType::VE_RIGHT);
			}
			else if (Value < -0.2f)
			{
				lockNavigateInteractiveUI = true;
				lockNavigateInteractiveUIElapsed = 0.0f;
				InteractiveUI->Navigate(EDirectionType::VE_LEFT);
			}
		}
	}

	if (!bIsInteractiveUIOpened)
	{
		if (myCharacter != nullptr)
		{
			myCharacter->MoveRight(Value);
		}

		/*if (((myCharacter == nullptr)) || (myCharacter->GetCurrentGesture() != EGestureType::VE_NONE)) return;

		if (Value != 0.0f)
		{
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement in that direction
			myCharacter->AddMovementInput(Direction, Value);
		}*/
	}

}


void ALobbyPlayerController::MoveForward(float Value)
{
	if (Value == 0.0f) return;

	if (bIsInteractiveUIOpened)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::MoveRight] bIsInteractiveUIOpened %f"), Value);

		if (InteractiveUI != nullptr)
		{
			if (lockNavigateInteractiveUI) return;

			if (Value >= 0.2f)
			{
				lockNavigateInteractiveUI = true;
				lockNavigateInteractiveUIElapsed = 0.0f;
				InteractiveUI->Navigate(EDirectionType::VE_UP);
			}
			else if (Value < -0.2f)
			{
				lockNavigateInteractiveUI = true;
				lockNavigateInteractiveUIElapsed = 0.0f;
				InteractiveUI->Navigate(EDirectionType::VE_DOWN);
			}
		}
	}

	if (!bIsInteractiveUIOpened)
	{
		if (myCharacter != nullptr)
		{
			myCharacter->MoveForward(Value);
		}


		/*if (((myCharacter == nullptr)) ||(myCharacter->GetCurrentGesture() != EGestureType::VE_NONE)) return;

		if (Value != 0.0f)
		{
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			myCharacter->AddMovementInput(Direction, Value);
		}*/
	}
}

void ALobbyPlayerController::Inspect()
{
	if (myCharacter == nullptr) return;

	myCharacter->HandleInspectInput();
}

void ALobbyPlayerController::Interact()
{
	if (myCharacter == nullptr) return;


	myCharacter->HandleInteractInput();
}

///////////// CHARACTER INPUT ////////////////



void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, AvalaibleCharacters);

}