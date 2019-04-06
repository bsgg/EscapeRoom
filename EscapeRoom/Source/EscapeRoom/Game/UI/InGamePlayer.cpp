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

	SelectedObjectID = "NONE";

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
	HideMessage();
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

void UInGamePlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	if (bTypewriterMessage)
	{
		CurrentTimeBetweenLetters += InDeltaTime;

		if (CurrentTimeBetweenLetters >= DelayBetweenLetters)
		{
			CurrentMessage += MessageToTypewrite[LetterIndex];

			InGameMessageText->SetText(FText::FromString(CurrentMessage));

			LetterIndex += 1;

			if (LetterIndex >= MessageToTypewrite.Len())
			{
				CurrentTimeBetweenLetters = 0.0f;
				bTypewriterMessage = false;

				bWaitingToHideMessage = true;
			}
		}
	}

	if (bWaitingToHideMessage)
	{
		CurrentTimeBetweenLetters += InDeltaTime;

		if (CurrentTimeBetweenLetters >= 3.0f)
		{
			CurrentTimeBetweenLetters = 0.0f;

			bWaitingToHideMessage = false;

			InGameMessagesBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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

void UInGamePlayer::ShowMessage(const FString& Text, float time)
{
	if (InGameMessageText == nullptr) return;

	//if (bWaitingToHideMessage) return;

	//UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::ShowMessage] Text %s "), *Text);

	InGameMessageText->SetText(FText::FromString(""));

	InGameMessagesBox->SetVisibility(ESlateVisibility::Visible);

	//InGameMessageText->SetText(FText::FromString(Text));

	LetterIndex = 0;

	CurrentMessage = "";

	MessageToTypewrite = Text;
	   	
	

	//LettersMessageLeft = MessageToTypewrite.Len();

	

	TotalTimeTypeWritting = DelayBetweenLetters * MessageToTypewrite.Len();

	CurrentTimeBetweenLetters = 0.0f;

	bTypewriterMessage = true;

	UE_LOG(LogTemp, Warning, TEXT("[UInGamePlayer::ShowMessage] Text: %s -  TotalTimeTypeWritting: %i "), *Text, TotalTimeTypeWritting);

	//if (time > 0.0f)
	//{
		//GetWorld()->GetTimerManager().SetTimer(MessageTimerHandle, this, &UInGamePlayer::HideMessage, time, false);
	//}

	//bWaitingToHideMessage = true;
}



void UInGamePlayer::HideMessage()
{
	if (InGameMessageText == nullptr) return;

	if (bWaitingToHideMessage)
	{
		InGameMessagesBox->SetVisibility(ESlateVisibility::Hidden);

		bWaitingToHideMessage = false;
		GetWorld()->GetTimerManager().ClearTimer(MessageTimerHandle);
	}
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

			SelectedObjectID = Object.ID;

			SelectedItemIcon->SetBrushFromTexture(Object.Thumbnail);
			SelectedItemIcon->SetVisibility(ESlateVisibility::Visible);

			break;
		}
		else
		{
			Slots[i]->UnHighlight();
		}
	}
}

void UInGamePlayer::RemoveObjectFromSlot(const FName& ObjID)
{
	int idToRemove = -1;
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->GetObjectSlot().ID == ObjID)
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
	}
}

void UInGamePlayer::NavigateInventory(EDirectionType Direction)
{
	if ((!bControlsVisible) || (numberObjectsInventory == 0)) return;
	

	if (Direction == EDirectionType::VE_LEFT)
	{
		CurrentSlotIndex -= 1;

		// Last element
		if (CurrentSlotIndex < 0)
		{
			CurrentSlotIndex = numberObjectsInventory - 1;
		}
	}
	else if (Direction == EDirectionType::VE_RIGHT)
	{
		CurrentSlotIndex += 1;
		
		// Last element
		if (CurrentSlotIndex >= numberObjectsInventory)
		{
			CurrentSlotIndex = 0;
		}
	}

	if ((CurrentSlotIndex >= 0) && (CurrentSlotIndex < numberObjectsInventory))
	{
		SelectedItemIcon->SetVisibility(ESlateVisibility::Visible);
		SelectedItemIcon->SetBrushFromTexture(Slots[CurrentSlotIndex]->GetObjectSlot().Thumbnail);
		SelectedObjectID = Slots[CurrentSlotIndex]->GetObjectSlot().ID;		
	}	
}

FName UInGamePlayer::GetSelectedItem()
{
	return SelectedObjectID;
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
	bControlsVisible = true;

	SelectedObjectID = "NONE";
	SelectedItemIcon->SetVisibility(ESlateVisibility::Hidden);

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

	bControlsVisible = false;
}

void UInGamePlayer::DebugLog(const FString& Text)
{
	if (DebugText == nullptr) return;

	DebugText->SetText(FText::FromString(Text));
}
