// Fill out your copyright notice in the Description page of Project Settings.

#include "InGamePlayer.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "Lobby/LobbyPlayerController.h"


bool UInGamePlayer::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;


	APlayerController* PC = GetOwningPlayer();
	if (PC == nullptr) return false;

	PlayerController = Cast<ALobbyPlayerController>(PC);
	if (PlayerController == nullptr) return false;

	// Set Debug Messages
	if (PlayerController->HasAuthority())
	{
		SetInGameMessage(FText::FromString("Server"));
	}
	else
	{
		SetInGameMessage(FText::FromString("Client"));
	}

	return true;

}

void UInGamePlayer::SetPortrait(ECharacterType Character)
{
	switch (Character)
	{
		case ECharacterType::VE_CAT:
			RacoonPortrait->SetVisibility(ESlateVisibility::Hidden);
			CatPortrait->SetVisibility(ESlateVisibility::Visible);
		break;
		case ECharacterType::VE_RACOON:
			RacoonPortrait->SetVisibility(ESlateVisibility::Visible);
			CatPortrait->SetVisibility(ESlateVisibility::Hidden);
		break;
		case ECharacterType::VE_NONE:
			RacoonPortrait->SetVisibility(ESlateVisibility::Hidden);
			CatPortrait->SetVisibility(ESlateVisibility::Hidden);
		break;
	}

}

void UInGamePlayer::SetInGameMessage(FText Message)
{
	if (InGameMessages == nullptr) return;
	InGameMessages->SetText(Message);
}