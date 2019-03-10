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

	SetVisibility(ESlateVisibility::Visible);		

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
		SelectedSlotID = 0;
		SelectedColumnSlot = 0;
		SelectedRowSlot = 0;
		Slots[SelectedSlotID]->Select();

		SelectedObject->Show();
		SelectedObject->SetObjectSlot(Objects[SelectedSlotID]);
		SelectedObject->UnSelect();
	}
	else
	{
		SelectedObject->Hide();
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
	if ((SelectedSlotID > -1) && (SelectedSlotID < ObjectNumberInInventory))
	{
		Slots[SelectedSlotID]->UnSelect();
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

	// Clamp selected column Row
	SelectedRowSlot = FMath::Clamp(SelectedRowSlot, 0, RowNumber - 1);
	SelectedColumnSlot = FMath::Clamp(SelectedColumnSlot, 0, ColumnNumber - 1);	

	SelectedSlotID = SelectedColumnSlot + (SelectedRowSlot * ColumnNumber);
	SelectedSlotID = FMath::Clamp(SelectedSlotID, 0, ObjectNumberInInventory - 1);

	Slots[SelectedSlotID]->Select();
	SelectedObject->SetObjectSlot(Slots[SelectedSlotID]->GetObjectSlot());

	if (DescriptionSlot != nullptr)
	{
		FText DescSlot = Slots[SelectedSlotID]->GetObjectSlot().Description;
		DescriptionSlot->SetText(DescSlot);
	}
}