// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryUI.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "InventorySlot.h"

bool UInventoryUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	const TArray<UPanelSlot*> GridSlots = InventoryGrid->GetSlots();

	for (int i = 0; i < GridSlots.Num(); i++)
	{
		UInventorySlot* Slot = Cast<UInventorySlot>(GridSlots[i]);
		if (Slot != nullptr)
		{
			Slots.Add(Slot);
		}
	}
	
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