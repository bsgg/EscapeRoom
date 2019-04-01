// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"

#include "LobbyMenu.h"
#include "LobbyGameMode.h"
#include "Game/EscapeRoomPlayerState.h"

#include "Game/Components/InventoryComponent.h"
#include "Game/GameLogic/Objects/ObjectHelper.h"

#include "Game/RoomGameMode.h"
#include "Game/UI/InGamePlayer.h"
#include "Game/UI/InventoryUI.h"
#include "Game/GameLogic/InteractiveBase.h"


#include "UObject/ConstructorHelpers.h"
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
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind inventory
	InputComponent->BindAction("NavigateInventoryLeft", IE_Pressed, this, &ALobbyPlayerController::NavigateInventoryLeft);
	InputComponent->BindAction("NavigateInventoryRight", IE_Pressed, this, &ALobbyPlayerController::NavigateInventoryRight);
	InputComponent->BindAction("InputToggleInventory", IE_Pressed, this, &ALobbyPlayerController::ToggleInventory);
}


///////////// LOBBY IMPLEMENTATION ////////////////
void ALobbyPlayerController::Client_Initialize_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_Initialize]"));

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
			//InGameUI->SetInGameMessage(FText::FromString(Message));
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
	InGameUI->HideMessage();
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
///////////// GAMEPLAY UI IMPLEMENTATION ////////////////


///////////// GAMEPLAY INVENTORY IMPLEMENTATION ////////////////
void ALobbyPlayerController::NavigateInventoryLeft()
{
	if (InGameUI == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::NavigateInventoryLeft] "));

	InGameUI->NavigateInventory(EDirectionType::VE_LEFT);
}

void ALobbyPlayerController::NavigateInventoryRight()
{
	if (InGameUI == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::NavigateInventoryRight] "));

	InGameUI->NavigateInventory(EDirectionType::VE_RIGHT);
}

void ALobbyPlayerController::ToggleInventory()
{
	if (InGameUI == nullptr) return;

	InGameUI->ToggleInventory();
}

FName ALobbyPlayerController::GetSelectedItem()
{
	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::GetSelectedItem] "));

	if (InGameUI == nullptr) return "NONE";

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::GetSelectedItem] %s "), *InGameUI->GetSelectedItem().ToString());

	return InGameUI->GetSelectedItem();
}

void ALobbyPlayerController::Client_RemoveObjectFromSlot_Implementation(const FName& ObjectID)
{
	if (InGameUI == nullptr) return;

	InGameUI->RemoveObjectFromSlot(ObjectID);
}

///////////// GAMEPLAY INVENTORY IMPLEMENTATION ////////////////



void ALobbyPlayerController::AddItemToInventory(const FName& ObjID)
{
	if (InventoryComponent == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::AddItemToInventory] ObjID %s"), *ObjID.ToString());

	FObjectInteraction* NewObject = UObjectHelper::GetObjectByID(ObjID);
	
	if (NewObject == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::AddItemToInventory] New Object added %s"), *NewObject->ID.ToString());

	InventoryComponent->AddObject(ObjID, *NewObject);

	if (InGameUI == nullptr) return;

	InGameUI->AddObjectToSlot(*NewObject);
}

void ALobbyPlayerController::RemoveItemFromInventory(const FName& ObjID)
{
	if (InventoryComponent == nullptr) return;

	InventoryComponent->RemoveObject(ObjID);

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::RemoveItemFromInventory] ObjID to remove %s"), *ObjID.ToString());

	if (InGameUI == nullptr) return;

	InGameUI->RemoveObjectFromSlot(ObjID);
}




void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, AvalaibleCharacters);

}