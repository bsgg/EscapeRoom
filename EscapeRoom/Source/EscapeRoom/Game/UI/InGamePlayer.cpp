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
	if (DebugText != nullptr)
	{
		if (PlayerController->HasAuthority())
		{
			DebugText->SetText(FText::FromString("Server"));

		}
		else
		{
			DebugText->SetText(FText::FromString("Client"));

		}
	}
	HideMessages();
	HideControls();
	
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
			RacoonPortraitIcon->SetVisibility(ESlateVisibility::Hidden);
			CatPortraitIcon->SetVisibility(ESlateVisibility::Visible);
		break;
		case ECharacterType::VE_RACOON:
			RacoonPortraitIcon->SetVisibility(ESlateVisibility::Visible);
			CatPortraitIcon->SetVisibility(ESlateVisibility::Hidden);
		break;
		case ECharacterType::VE_NONE:
			RacoonPortraitIcon->SetVisibility(ESlateVisibility::Hidden);
			CatPortraitIcon->SetVisibility(ESlateVisibility::Hidden);
		break;
	}
}

void UInGamePlayer::ShowMessage(const FString& Text)
{
	if (InGameMessageText == nullptr) return;

	InGameMessagesBox->SetVisibility(ESlateVisibility::Visible);

	InGameMessageText->SetText(FText::FromString(Text));
}



void UInGamePlayer::HideMessage()
{
	if (InGameMessageText == nullptr) return;

	InGameMessagesBox->SetVisibility(ESlateVisibility::Hidden);
}


void UInGamePlayer::SetInGameMessage(FText Message)
{
	if (InGameMessageText == nullptr) return;
	InGameMessageText->SetText(Message);
}

void UInGamePlayer::HideMessages()
{
	if (InGameMessagesBox == nullptr) return;

	InGameMessagesBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInGamePlayer::ShowMessages()
{
	if (InGameMessagesBox == nullptr) return;

	InGameMessagesBox->SetVisibility(ESlateVisibility::Visible);
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

void UInGamePlayer::RemoveObjectFromSlot(FName ObjectID)
{
	int idToRemove = -1;
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->GetObjectSlot().ID == ObjectID)
		{
			idToRemove = i;

			Slots[i]->SetToDefault();

			Slots[i]->UnHighlight();

			SelectedItemIcon->SetVisibility(ESlateVisibility::Hidden);			

			numberObjectsInventory -= 1;

			if (numberObjectsInventory < -1)
			{
				numberObjectsInventory = 0;
			}

			CurrentSlotIndex = -1;

			break;
		}		
	}

	if (idToRemove > -1)
	{
		for (int i = idToRemove+1; i < Slots.Num(); i++, idToRemove++)
		{
			if (Slots[i]->IsEmpty())
			{
				Slots[idToRemove]->SetToDefault();
			}
			else
			{
				Slots[idToRemove]->SetObjectSlot(Slots[i]->GetObjectSlot());
			}
			

		}


		//for (int i = idToRemove, iNext = idToRemove +1; i < Slots.Num(); i++)
		//{
			//if (iNext <= numberObjectsInventory)
			//{
				//Slots[i]->SetObjectSlot(Slots[iNext]->GetObjectSlot());
				//Slots[iNext]->SetToDefault();
			//}
			//else
			//{
				//if (iNext < Slots.Num())
				//{
					//Slots[iNext]->SetToDefault();
				//}
				//Slots[i]->SetToDefault();
			//}
		//}
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



void UInGamePlayer::ShowControls(bool ShowInventoryIcon, bool ShowInspectIcon, bool ShowUseIcon)
{
	if ((ControlsBox == nullptr) || (InventoryIconBox == nullptr) || (UseIconBox == nullptr) || (InspectIconBox == nullptr)) return;

	ControlsBox->SetVisibility(ESlateVisibility::Visible);

	if (ShowInventoryIcon)
	{
		InventoryIconBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InventoryIconBox->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ShowInspectIcon)
	{
		InspectIconBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InspectIconBox->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ShowUseIcon)
	{
		UseIconBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UseIconBox->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UInGamePlayer::HideControls()
{
	ControlsBox->SetVisibility(ESlateVisibility::Hidden);
}



void UInGamePlayer::ShowInventoryIcon()
{
	if (InventoryIconBox == nullptr) return;

	InventoryIconBox->SetVisibility(ESlateVisibility::Visible);

}
void UInGamePlayer::HideInventoryIcon()
{
	if (InventoryIconBox == nullptr) return;

	InventoryIconBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInGamePlayer::ShowInspectIcon()
{
	if (InspectIconBox == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::ShowInspectIcon] "));

	InspectIconBox->SetVisibility(ESlateVisibility::Visible);
}
void UInGamePlayer::HideInspectIcon()
{
	if (InspectIconBox == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::HideInspectIcon] "));
	InspectIconBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInGamePlayer::ShowUseIcon()
{
	if (UseIconBox == nullptr) return;

	UseIconBox->SetVisibility(ESlateVisibility::Visible);
}

void UInGamePlayer::HideUseIcon()
{
	if (UseIconBox == nullptr) return;

	UseIconBox->SetVisibility(ESlateVisibility::Hidden);
}


void UInGamePlayer::DebugLog(const FString& Text)
{
	if (DebugText == nullptr) return;

	DebugText->SetText(FText::FromString(Text));
}
