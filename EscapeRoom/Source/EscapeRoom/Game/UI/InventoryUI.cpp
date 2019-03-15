// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryUI.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "InventorySlot.h"

bool UInventoryUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	Slots.Add(Slot_0);
	Slots.Add(Slot_1);
	Slots.Add(Slot_2);
	Slots.Add(Slot_3);
	Slots.Add(Slot_4);
	Slots.Add(Slot_5);

	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->Hide();
	}

	return true;
}

void UInventoryUI::Show(const TArray<FObjectInteraction>& Objects)
{
	ObjectNumberInInventory = Objects.Num();		

	SetVisibility(ESlateVisibility::Visible);	

	
	//CombineSlotA->SetIndex(-1);


	// Show all slots
	for (int i = 0, iObject = 0; i < Slots.Num(); i++, iObject++)
	{
		Slots[i]->SetIndex(iObject);

		if (iObject < Objects.Num())
		{
			Slots[i]->SetObjectSlot(Objects[iObject]);

			Slots[i]->Show();

			Slots[i]->UnHighlight();
		}
		else
		{
			Slots[i]->SetToDefault();

			Slots[i]->Show();
		}
	}	

	CurrentSlotIndex = -1;
	

	CombineSlotA->SetToDefault();
	CombineSlotB->SetToDefault();

	bReadyToCombine = false;
	ObjectASlotIndex = -1;
	ObjectBSlotIndex = -1;
	

	if (ObjectNumberInInventory > 0)
	{
		CurrentSlotIndex = 0;
		Slots[CurrentSlotIndex]->Highlight();

	}

	if (DescriptionSlot != nullptr)
	{
		DescriptionSlot->SetText(FText::FromString(""));
	}
}
void UInventoryUI::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryUI::Navigate(EDirectionType Direction)
{
	if (ObjectNumberInInventory == 0) return;

	// Deselect current slot
	if ((CurrentSlotIndex > -1) && (CurrentSlotIndex < ObjectNumberInInventory))
	{
		Slots[CurrentSlotIndex]->UnHighlight();
	}

	switch (Direction)
	{		
		case EDirectionType::VE_RIGHT:
			CurrentSlotIndex += 1;
		break;
		case EDirectionType::VE_LEFT:
			CurrentSlotIndex -= 1;
		break;	
	}

	// Clamp selected column and selected row
	CurrentSlotIndex = FMath::Clamp(CurrentSlotIndex, 0, ObjectNumberInInventory - 1);

	Slots[CurrentSlotIndex]->Highlight();

	if (ObjectASlotIndex != -1)
	{
		//ObjectBSlotIndex = CurrentSlotIndex;

		//FString NameObjectA = Slots[ObjectASlotIndex]->GetObjectSlot().Name.ToString();
		//FString NameObjectB = Slots[ObjectBSlotIndex]->GetObjectSlot().Name.ToString();
		//FString Text = NameObjectA + " Combine with " + NameObjectB;

		//DescriptionSlot->SetText(FText::FromString(""));

		//bReadyToCombine = true;
	}
	else
	{
		//FText Desc = Slots[CurrentSlotIndex]->GetObjectSlot().Description;
		
	}
	
	DescriptionSlot->SetText(FText::FromString(""));


	// TODO: If not object A selected show description of CurrentSlotIndex

	// TODO: If object A selected set B description combine with...
}


void UInventoryUI::OnSelectItem()
{
	if (ObjectNumberInInventory == 0) return;
	
	if (ObjectASlotIndex == -1) // Object selected
	{
		ObjectASlotIndex = CurrentSlotIndex;
		CombineSlotA->SetObjectSlot(Slots[CurrentSlotIndex]->GetObjectSlot());
		CombineSlotA->Highlight();

		FString NameObjectA = CombineSlotA->GetObjectSlot().Name.ToString();
		FString Text = NameObjectA + " Combine with ";
		CombineText->SetText(FText::FromString(Text));
	}
	else if (ObjectASlotIndex > -1)
	{
		if (ObjectASlotIndex == CurrentSlotIndex) // Same selection, remove both objects
		{
			ObjectASlotIndex = -1;
			ObjectBSlotIndex = -1;
			CombineSlotA->SetToDefault();
			CombineSlotB->SetToDefault();
		}
		else // Select second object
		{
			ObjectBSlotIndex = CurrentSlotIndex;
			CombineSlotB->SetObjectSlot(Slots[ObjectBSlotIndex]->GetObjectSlot());
			CombineSlotB->Highlight();

			FString NameObjectA = CombineSlotA->GetObjectSlot().Name.ToString();
			FString NameObjectB= CombineSlotB->GetObjectSlot().Name.ToString();
			FString Text = NameObjectA + " Combine with " + NameObjectB;
			CombineText->SetText(FText::FromString(Text));
		}
	}

}

FName UInventoryUI::GetObjectIDToCombineA()
{
	if (ObjectNumberInInventory == 0) return "NONE";

	if ((ObjectASlotIndex == -1) || (ObjectASlotIndex >= ObjectNumberInInventory)) return "NONE";

	return Slots[ObjectASlotIndex]->GetObjectSlot().ID;
	
}
FName UInventoryUI::GetObjectIDToCombineB()
{
	if (ObjectNumberInInventory == 0) return "NONE";

	if ((ObjectBSlotIndex == -1) || (ObjectBSlotIndex >= ObjectNumberInInventory)) return "NONE";

	return Slots[ObjectBSlotIndex]->GetObjectSlot().ID;
}

bool UInventoryUI::IsReadyToCombine() const
{
	return ((ObjectASlotIndex != -1) && (ObjectBSlotIndex != -1));
}