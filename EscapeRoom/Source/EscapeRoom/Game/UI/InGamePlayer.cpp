// Fill out your copyright notice in the Description page of Project Settings.

#include "InGamePlayer.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "Components/Image.h"
#include "Lobby/LobbyPlayerController.h"
#include "InventoryUI.h"
#include "InventorySlot.h"


bool UInGamePlayer::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	// Get Hold Player Controller
	APlayerController* PC = GetOwningPlayer();
	if (PC == nullptr) return false;

	PlayerController = Cast<ALobbyPlayerController>(PC);
	if (PlayerController == nullptr) return false;

	UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::Initialize] - Player Controller Not Null"));

	// Set Debug Messages
	if (PlayerController->HasAuthority())
	{
		SetInGameMessage(FText::FromString("Server"));

	}
	else
	{
		SetInGameMessage(FText::FromString("Client"));

	}
	//HideMessages();
	
	Slots.Add(Slot_0);
	Slots.Add(Slot_1);
	Slots.Add(Slot_2);
	Slots.Add(Slot_3);
	Slots.Add(Slot_4);

	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetToDefault();
	}

	SelectedItemIcon->SetVisibility(ESlateVisibility::Hidden);

	if (InventoryGrid != nullptr)
	{
		InventoryGrid->SetVisibility(ESlateVisibility::Hidden);

		bInventoryVisible = false;
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
	if (!bInventoryVisible)
	{
		InventoryGrid->SetVisibility(ESlateVisibility::Visible);

		bInventoryVisible = true;
	}

	if (numberObjectsInventory == 0) return;

	// None direction, end navigation
	if (Direction == EDirectionType::VE_NONE)
	{
		if ((CurrentSlotIndex >= 0) && (CurrentSlotIndex < numberObjectsInventory))
		{
			SelectedItemIcon->SetBrushFromTexture(Slots[CurrentSlotIndex]->GetObjectSlot().Thumbnail);
			SelectedItemIcon->SetVisibility(ESlateVisibility::Visible);

			PlayerController->Client_OnSelectItemInInventory(Slots[CurrentSlotIndex]->GetObjectSlot());			
			Slots[CurrentSlotIndex]->UnHighlight();			
		}

		return;
	}

	if ((CurrentSlotIndex >= 0) && (CurrentSlotIndex < numberObjectsInventory))
	{
		Slots[CurrentSlotIndex]->UnHighlight();
	}

	if (Direction == EDirectionType::VE_LEFT)
	{
		CurrentSlotIndex -= 1;

		// Last element
		if (CurrentSlotIndex < 0)
		{
			//CurrentSlotIndex = numberObjectsInventory - 1;
			CurrentSlotIndex = -1;

			SelectedItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (Direction == EDirectionType::VE_RIGHT)
	{
		CurrentSlotIndex += 1;
		
		// Last element
		if (CurrentSlotIndex >= numberObjectsInventory)
		{
			CurrentSlotIndex = numberObjectsInventory;

			SelectedItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if ((CurrentSlotIndex >= 0) && (CurrentSlotIndex < numberObjectsInventory))
	{
		Slots[CurrentSlotIndex]->Highlight();
	}	
}

void UInGamePlayer::ToggleInventory()
{
	if (InventoryGrid == nullptr) return;

	if (!bInventoryVisible)
	{
		InventoryGrid->SetVisibility(ESlateVisibility::Visible);

		bInventoryVisible = true;
	}
	else
	{
		InventoryGrid->SetVisibility(ESlateVisibility::Hidden);

		bInventoryVisible = false;
	}
}
