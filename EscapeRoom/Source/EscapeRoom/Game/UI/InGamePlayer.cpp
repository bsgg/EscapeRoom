// Fill out your copyright notice in the Description page of Project Settings.

#include "InGamePlayer.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "Lobby/LobbyPlayerController.h"
#include "InventoryUI.h"
#include "InventorySlot.h"


bool UInGamePlayer::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	HideMessages();
	HideSelectedObject();

	

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

void UInGamePlayer::HideMessages()
{
	if (MessagesBox == nullptr) return;

	MessagesBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInGamePlayer::ShowMessages()
{
	if (MessagesBox == nullptr) return;

	MessagesBox->SetVisibility(ESlateVisibility::Visible);
}


void UInGamePlayer::ShowInventory(const TArray<FObjectInteraction>& Objects)
{
	if (Inventory == nullptr) return;
	Inventory->Show(Objects);
}

void UInGamePlayer::HideInventory()
{
	if (Inventory == nullptr) return;
	Inventory->Hide();
}

void UInGamePlayer::NavigateInventory(EDirectionType Direction)
{
	if (Inventory == nullptr) return;

	Inventory->Navigate(Direction);
}

void UInGamePlayer::OnSelectItemInventory()
{
	if (Inventory == nullptr) return;

	Inventory->OnSelectItem();
}


void UInGamePlayer::ShowSelectedObject(const FObjectInteraction& Object)
{
	if (SelectedObject == nullptr) return;
	SelectedObject->SetObjectSlot(Object);
	SelectedObject->Show();
}

void UInGamePlayer::HideSelectedObject()
{
	if (SelectedObject == nullptr) return;
	SelectedObject->Hide();
}



void UInGamePlayer::AddObjectToInventory(const FObjectInteraction& Object)
{
	// Look for an inactive slot
	/*for (int i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i]->IsEmpty())
		{
			InventorySlots[i]->SetImageSlot(Object.Thumbnail);
			return;
		}
	}*/
}

void UInGamePlayer::UpdateInventory(const TArray<FObjectInteraction>& Objects)
{
	// Remove all slots
	/*for (int i = 0; i < InventorySlots.Num(); i++)
	{
		InventorySlots[i]->SetImageSlot(nullptr);
	}

	for (int i = 0; i < Objects.Num(); i++)
	{
		if (i < InventorySlots.Num())
		{
			InventorySlots[i]->SetImageSlot(Objects[i].Thumbnail);
		}
	}*/
}