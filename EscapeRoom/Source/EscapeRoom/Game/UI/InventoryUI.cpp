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

	if (DescriptionSlot != nullptr)
	{
		DescriptionSlot->SetText(FText::FromString(""));
	}
}
void UInventoryUI::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}