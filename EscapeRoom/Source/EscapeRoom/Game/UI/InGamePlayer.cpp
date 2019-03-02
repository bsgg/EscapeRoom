// Fill out your copyright notice in the Description page of Project Settings.

#include "InGamePlayer.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "Lobby/LobbyPlayerController.h"
#include "InventorySlot.h"


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


	InventorySlots.Add(Slot0);
	InventorySlots.Add(Slot1);
	InventorySlots.Add(Slot2);
	InventorySlots.Add(Slot3);
	InventorySlots.Add(Slot4);

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


void UInGamePlayer::AddObjectToInventory(const FObjectInteraction& Object)
{
	// Look for an inactive slot
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i]->IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::AddObjectToInventory] Free Slot at: %i"), i);

			InventorySlots[i]->SetImageSlot(Object.Thumbnail);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::AddObjectToInventory] Not free slot found"));
}