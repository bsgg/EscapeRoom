// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"

#include "LobbyMenu.h"
#include "LobbyGameMode.h"
#include "Game/EscapeRoomPlayerState.h"

#include "Game/RoomGameMode.h"
#include "Game/UI/InGamePlayer.h"

#include "UObject/ConstructorHelpers.h"
#include "UnrealNetwork.h"

ALobbyPlayerController::ALobbyPlayerController()
{

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
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_InitializeRoom] %  - MyCharacter: %s"), *Authority, *TCharacter);

		if (InGameUI != nullptr)
		{
			InGameUI->SetPortrait(ESPlayerScape->SelectedCharacter);
			InGameUI->SetInGameMessage(FText::FromString(Message));
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

void ALobbyPlayerController::Client_UpdateInGameMessageUI_Implementation(const FString& Text, bool hideMessages)
{
	// Create UI
	if (InGameUI == nullptr) return;	

	if (hideMessages)
	{
		InGameUI->HideMessages();
	}
	else
	{
		InGameUI->ShowMessages();
		InGameUI->SetInGameMessage(FText::FromString(Text));
	}
}

/*
void ALobbyPlayerController::Client_UpdateInGameSlotUI_Implementation(const FObjectInteraction& Object)
{
	if (InGameUI == nullptr) return;

	InGameUI->AddObjectToInventory(Object);
}

void ALobbyPlayerController::Client_UpdateInGameInventoryUI_Implementation(const TArray<FObjectInteraction>& Objects)
{
	if (InGameUI == nullptr) return;

	InGameUI->UpdateInventory(Objects);
}
*/

void ALobbyPlayerController::Client_OpenInventory_Implementation(const TArray<FObjectInteraction>& Objects)
{
	if (InGameUI == nullptr) return;

	InGameUI->ShowInventory(Objects);
}

void ALobbyPlayerController::Client_CloseInventory_Implementation()
{
	if (InGameUI == nullptr) return;

	InGameUI->HideInventory();
}


void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, AvalaibleCharacters);

}