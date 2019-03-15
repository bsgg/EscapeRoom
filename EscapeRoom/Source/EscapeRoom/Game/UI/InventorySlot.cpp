// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Utils/Definitions.h"

bool UInventorySlot::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	return true;
}

void UInventorySlot::Show()
{
	SlotBox->SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlot::Hide()
{
	SlotBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::SetIndex(int in_index)
{
	index = in_index;
}

void UInventorySlot::SetObjectSlot(const FObjectInteraction& Object)
{
	ObjectSlot = Object;

	SetImageSlot(ObjectSlot.Thumbnail);
}

void UInventorySlot::SetToDefault()
{
	if (Thumbnail == nullptr) return;

	Thumbnail->SetVisibility(ESlateVisibility::Hidden);
}


void UInventorySlot::SetImageSlot(UTexture2D* Image)
{
	if ((Thumbnail == nullptr) || (Image == nullptr)) return;

	Thumbnail->SetBrushFromTexture(Image);
	
	Thumbnail->SetVisibility(ESlateVisibility::Visible);

	UnHighlight();
}


void UInventorySlot::Highlight()
{
	if (Thumbnail == nullptr) return;

	Thumbnail->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	//if ((Background == nullptr) || (Background1 == nullptr)) return;

	//Background->SetVisibility(ESlateVisibility::Hidden);
	//Background1->SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlot::UnHighlight()
{
	if (Thumbnail == nullptr) return;

	Thumbnail->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));

	//if ((Background == nullptr) || (Background1 == nullptr)) return;


	//Background->SetVisibility(ESlateVisibility::Visible);
	//Background1->SetVisibility(ESlateVisibility::Hidden);
}


