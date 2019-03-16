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
	
	Slots.Add(Slot_0);
	Slots.Add(Slot_1);
	Slots.Add(Slot_2);
	Slots.Add(Slot_3);
	Slots.Add(Slot_4);

	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetToDefault();
	}

	SelectedItem->SetToDefault();

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


void UInGamePlayer::AddObjectToSlot(FObjectInteraction Object)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->IsEmpty())
		{
			Slots[i]->SetObjectSlot(Object);

			Slots[i]->UnHighlight();

			numberObjectsInventory += 1;

			break;
		}
		else
		{
			Slots[i]->UnHighlight();
		}
	}
}

void UInGamePlayer::NavigateInventory(EDirectionType Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::NavigateInventory] - numberObjectsInventory %i"), numberObjectsInventory);

	if (numberObjectsInventory == 0) return;

	if ((CurrentSlotIndex >= 0) && (CurrentSlotIndex < numberObjectsInventory))
	{
		Slots[CurrentSlotIndex]->UnHighlight();
	}

	if (Direction == EDirectionType::VE_LEFT)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::NavigateInventory] ToLeft"));

		CurrentSlotIndex -= 1;

		if (CurrentSlotIndex < 0)
		{
			CurrentSlotIndex = numberObjectsInventory - 1;
		}
	}
	else if (Direction == EDirectionType::VE_RIGHT)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::NavigateInventory] ToRight"));

		CurrentSlotIndex += 1;

		if (CurrentSlotIndex >= numberObjectsInventory)
		{
			CurrentSlotIndex = 0;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::NavigateInventory] CurrentSlotIndex %i"), CurrentSlotIndex);

	if ((CurrentSlotIndex >= 0) && (CurrentSlotIndex < numberObjectsInventory))
	{
		Slots[CurrentSlotIndex]->Highlight();
		SelectedItem->SetObjectSlot(Slots[CurrentSlotIndex]->GetObjectSlot());
	}	
}

void UInGamePlayer::EndNavigateInventory()
{
	if ((CurrentSlotIndex >= 0) && (CurrentSlotIndex < numberObjectsInventory))
	{
		Slots[CurrentSlotIndex]->UnHighlight();
	}
}




void UInGamePlayer::ShowInventory(const TArray<FObjectInteraction>& Objects)
{
	//if (Inventory == nullptr) return;
	//Inventory->Show(Objects);
}

void UInGamePlayer::HideInventory()
{
	//if (Inventory == nullptr) return;
	//Inventory->Hide();
}



void UInGamePlayer::OnSelectItemInventory()
{
	//if (Inventory == nullptr) return;

	//Inventory->OnSelectItem();
}


void UInGamePlayer::ShowSelectedObject(const FObjectInteraction& Object)
{
	//if (SelectedObject == nullptr) return;
	//SelectedObject->SetObjectSlot(Object);
	//SelectedObject->Show();
}

void UInGamePlayer::HideSelectedObject()
{
	//if (SelectedObject == nullptr) return;
	//SelectedObject->Hide();
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