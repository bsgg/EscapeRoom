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


	CombineSlotA->SetToDefault();

	CombineSlotA->SetIndex(-1);


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
	bReadyToCombine = false;
	ObjectBSlotIndex = -1;

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

	if (CombineSlotA->GetIndex() != -1)
	{
		ObjectBSlotIndex = CurrentSlotIndex;

		FString NameObjectA = CombineSlotA->GetObjectSlot().Name.ToString();
		FString NameObjectB = Slots[ObjectBSlotIndex]->GetObjectSlot().Name.ToString();
		FString Text = NameObjectA + " Combine with " + NameObjectB;

		DescriptionSlot->SetText(FText::FromString(Text));

		bReadyToCombine = true;
	}
	else
	{
		FText Desc = Slots[CurrentSlotIndex]->GetObjectSlot().Description;
		DescriptionSlot->SetText(Desc);
	}
	

	// TODO: If not object A selected show description of CurrentSlotIndex

	// TODO: If object A selected set B description combine with...
}


void UInventoryUI::OnSelectItem()
{
	if (ObjectNumberInInventory == 0) return;

	if (CombineSlotA->GetIndex() == -1)
	{
		CombineSlotA->SetIndex(CurrentSlotIndex);
		CombineSlotA->SetObjectSlot(Slots[CurrentSlotIndex]->GetObjectSlot());
		CombineSlotA->Highlight();

		FString NameObjectA = CombineSlotA->GetObjectSlot().Name.ToString();
		FString Text = NameObjectA + " Combine with ";

		DescriptionSlot->SetText(FText::FromString("Text"));
	}
	else if (CombineSlotA->GetIndex() == CurrentSlotIndex)
	{
		ObjectBSlotIndex = -1;
		CombineSlotA->SetToDefault();
		CombineSlotA->SetIndex(-1);

		//CombineSlotB->SetToDefault();
		//CombineSlotB->SetIndex(-1);		
	}
	/*else if (CombineSlotB->GetIndex() == -1)
	{
		CombineSlotB->SetIndex(CurrentSlotIndex);
		CombineSlotB->SetObjectSlot(Slots[CurrentSlotIndex]->GetObjectSlot());
		CombineSlotB->Highlight();

		// TODO:: Check combination
	}*/
}

FName UInventoryUI::GetObjectIDToCombineA()
{
	if (CombineSlotA->GetIndex() != -1)
	{
		return CombineSlotA->GetObjectSlot().ID;
	}

	return "NONE";
}
FName UInventoryUI::GetObjectIDToCombineB()
{
	if (ObjectBSlotIndex != -1)
	{
		return Slots[ObjectBSlotIndex]->GetObjectSlot().ID;
	}

	return "NONE";
}

bool UInventoryUI::IsReadyToCombine() const
{
	return ((ObjectBSlotIndex != -1) && (CombineSlotA->GetIndex()));
}