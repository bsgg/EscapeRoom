// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"


bool ULobbyMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	UE_LOG(LogTemp, Warning, TEXT("[ULobbyMenu::Initialize]"));

	// Add On Click Events
	if (CatCharacterButton == nullptr) return false;
	CatCharacterButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnCatCharacterClicked);

	if (RacoonCharacterButton == nullptr) return false;
	RacoonCharacterButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnRacoonCharacterClicked);

	if (ReadyButton == nullptr) return false;
	ReadyButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnReadyButtonClicked);

	if (ExitButton == nullptr) return false;
	ExitButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnExitButtonClicked);


	// Get Hold Player Controller
	APlayerController* PC = GetOwningPlayer();
	if (PC == nullptr) return false;

	PlayerController = Cast<ALobbyPlayerController>(PC);
	if (PlayerController == nullptr) return false;

	// Set Debug Messages
	if (PlayerController->HasAuthority())
	{
		DebugText->SetText(FText::FromString("Server"));
	}
	else
	{
		DebugText->SetText(FText::FromString("Client"));
	}


	return true;

}

void ULobbyMenu::OnCatCharacterClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("[ULobbyMenu::OnCatCharacterClicked]"));

	OnCharacterClicked(ECharacterType::VE_CAT);
}


void ULobbyMenu::OnRacoonCharacterClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("[ULobbyMenu::OnRacoonCharacterClicked]"));

	OnCharacterClicked(ECharacterType::VE_RACOON);
}

void ULobbyMenu::OnCharacterClicked(ECharacterType CharacterType)
{
	if (PlayerController == nullptr) return;

	FString CharSelected = "Char Selected: " + GetEnumValueAsString<ECharacterType>("ECharacterType", CharacterType);
	if (PlayerController->HasAuthority())
	{
		DebugText->SetText(FText::FromString("Server: " + CharSelected));
	}
	else
	{
		DebugText->SetText(FText::FromString("Client: " + CharSelected));
	}

	PlayerController->Server_SelectCharacter(CharacterType);

}

void ULobbyMenu::OnReadyButtonClicked()
{
	ShowMessageBlocker(FText::FromString("Be ready, the game will start soon =)"));

	UE_LOG(LogTemp, Warning, TEXT("[ULobbyMenu::OnReadyButtonClicked] "));

	UWorld* World = this->GetWorld();
	if (World == nullptr) return;

	// Get Current Game Mode (only on server)
	ALobbyGameMode* LobbyGM = Cast<ALobbyGameMode>(World->GetAuthGameMode());
	if (World == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[ULobbyMenu::OnStartGame] LobbyGM->StartGame"));
	LobbyGM->StartGame();
}


void ULobbyMenu::SetActiveCharacterButton(ECharacterType CharacterType, bool Enable)
{
	switch (CharacterType)
	{
	case ECharacterType::VE_CAT:
	
		CatCharacterButton->SetIsEnabled(Enable);
		break;

	case ECharacterType::VE_RACOON:
		RacoonCharacterButton->SetIsEnabled(Enable);
		break;
	}
}

void ULobbyMenu::UpdateReadyState()
{
	if (PlayerController == nullptr) return;

	if (PlayerController->HasAuthority())
	{
		DebugText->SetText(FText::FromString("Server Ready to Start Session: "));
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ShowMessageBlocker(FText::FromString("Be ready, the game will start soon =)"));

		DebugText->SetText(FText::FromString("Client The session will start soon.. "));
	}
}


void ULobbyMenu::OnExitButtonClicked()
{
	// TODO: GO BACK TO MAIN MENU
	UWorld*	World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ConsoleCommand("quit");
}


void ULobbyMenu::ShowMessageBlocker(FText Message)
{
	if ((MessageBlockerWidget == nullptr) || (MessageBlockerText == nullptr)) return;

	MessageBlockerText->SetText(Message);

	MessageBlockerWidget->SetVisibility(ESlateVisibility::Visible);
	
}
void ULobbyMenu::HideMessageBlocker()
{
	if (MessageBlockerWidget == nullptr) return;

	MessageBlockerWidget->SetVisibility(ESlateVisibility::Hidden);
}