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

	// No objects to combine
	CombineSlotA->SetToDefault();
	CombineSlotB->SetToDefault();
	CombineSlotA->SetIndex(-1);
	CombineSlotB->SetIndex(-1);

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

	SelectedColumnSlot = -1;
	SelectedRowSlot = -1;
	CurrentSlotIndex = -1;
		   
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
		Slots[CurrentSlotIndex]->UnHighlight();
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
	Slots[CurrentSlotIndex]->Highlight();

	if (CombineSlotA->GetIndex() != -1)
	{
		FString NameObjectA = CombineSlotA->GetObjectSlot().Name.ToString();
		FString NameObjectB = Slots[CurrentSlotIndex]->GetObjectSlot().Name.ToString();
		FString Text = NameObjectA + " Combine with " + NameObjectB;

		DescriptionSlot->SetText(FText::FromString(Text));
	}
	else
	{
		FText Desc = Slots[CurrentSlotIndex]->GetObjectSlot().Description;
		DescriptionSlot->SetText(Desc);
	}
	

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
		CombineSlotA->SetToDefault();
		CombineSlotA->SetIndex(-1);

		CombineSlotB->SetToDefault();
		CombineSlotB->SetIndex(-1);		
	}
	/*else if (CombineSlotB->GetIndex() == -1)
	{
		CombineSlotB->SetIndex(CurrentSlotIndex);
		CombineSlotB->SetObjectSlot(Slots[CurrentSlotIndex]->GetObjectSlot());
		CombineSlotB->Highlight();

		// TODO:: Check combination
	}*/
}