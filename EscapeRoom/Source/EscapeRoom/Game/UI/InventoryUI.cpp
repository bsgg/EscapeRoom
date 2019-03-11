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
	Slots.Add(Slot_6);
	Slots.Add(Slot_7);
	Slots.Add(Slot_8);

	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->Hide();
	}

	return true;
}

void UInventoryUI::Show(const TArray<FObjectInteraction>& Objects)
{
	ObjectNumberInInventory = Objects.Num();	

	// No objects selected
	CombineSlotAIndex = -1;
	CombineSlotBIndex = -1;

	SetVisibility(ESlateVisibility::Visible);		

	SelectedObject->Hide();

	for (int i = 0, iObject = 0; i < Slots.Num(); i++, iObject++)
	{
		if (iObject < Objects.Num())
		{
			Slots[i]->SetObjectSlot(Objects[iObject]);

			Slots[i]->Show();
		}
		else
		{
			Slots[i]->Hide();
		}
	}

	

	if (ObjectNumberInInventory > 0)
	{
		SelectedColumnSlot = 0;
		SelectedRowSlot = 0;
		CurrentSlotIndex = 0;
		Slots[CurrentSlotIndex]->Select();
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
	// Deselect current slot
	if ((CurrentSlotIndex > -1) && (CurrentSlotIndex < ObjectNumberInInventory))
	{
		Slots[CurrentSlotIndex]->UnSelect();
	}

	switch (Direction)
	{
		case EDirectionType::VE_UP:
			SelectedRowSlot -= 1;
		break;
		case EDirectionType::VE_DOWN:
			SelectedRowSlot += 1;
		break;
		case EDirectionType::VE_RIGHT:
			SelectedColumnSlot += 1;
		break;
		case EDirectionType::VE_LEFT:
			SelectedColumnSlot -= 1;
		break;	
	}

	// Clamp selected column and selected row
	SelectedRowSlot = FMath::Clamp(SelectedRowSlot, 0, ROWS - 1);
	SelectedColumnSlot = FMath::Clamp(SelectedColumnSlot, 0, COLUMNS - 1);

	CurrentSlotIndex = GetClampedIndex();
	Slots[CurrentSlotIndex]->Select();

	// TODO: If not object A selected show description of CurrentSlotIndex

	// TODO: If object A selected set B description combine with...
}

int UInventoryUI::GetClampedIndex()
{
	int index = (SelectedColumnSlot + (SelectedRowSlot * COLUMNS));
	index = FMath::Clamp(index, 0, ObjectNumberInInventory - 1);
	return index;
}

void UInventoryUI::OnSelectItem()
{
	if (ObjectNumberInInventory == 0) return;

	// Object A Not selected, select this object
	if (CombineSlotAIndex == -1)
	{
		CombineSlotAIndex = GetClampedIndex(); 

		SelectedObject->SetObjectSlot(Slots[CombineSlotAIndex]->GetObjectSlot());		

		FText DescSlot = Slots[CombineSlotAIndex]->GetObjectSlot().Description;
		DescriptionSlot->SetText(DescSlot);

		SelectedObject->Show();
	}

	// Object A Selected
	if (CombineSlotAIndex != -1)
	{
		// Same ID, toggle selection
		int index = GetClampedIndex();

		if (index == CombineSlotAIndex)
		{
			Slots[CombineSlotAIndex]->UnSelect();
			SelectedObject->Hide();
			DescriptionSlot->SetText(FText::FromString(""));

			CombineSlotAIndex = -1;
		}
	}


	/*else
	{
		// First item is selected
		// Check if current one is the same item
		int index = GetClampedIndex();

		if (index != CombineSlotAIndex)
		{
			CombineSlotBIndex = index;

			FString ObjectBName = Slots[CombineSlotBIndex]->GetObjectSlot().Name.ToString();;

			FString ObjectAName = Slots[CombineSlotAIndex]->GetObjectSlot().Name.ToString();

			FString DesCombination = ObjectAName + " Combine with " + ObjectBName;

			DescriptionSlot->SetText(FText::FromString(DesCombination));
		}
	}*/

}