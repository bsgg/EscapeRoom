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

	SetTitleSlot(ObjectSlot.Name);	
}

void UInventorySlot::SetToDefault()
{
	if (Thumbnail == nullptr) return;

	Thumbnail->SetVisibility(ESlateVisibility::Hidden);

	SetTitleSlot(FText::FromString(""));

	UnHighlight();
}


void UInventorySlot::SetImageSlot(UTexture2D* Image)
{
	if ((Thumbnail == nullptr) || (Image == nullptr)) return;

	Thumbnail->SetBrushFromTexture(Image);

	Thumbnail->SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlot::SetTitleSlot(FText Title)
{
	if (TitleText == nullptr) return;

	TitleText->SetText(Title);
}


void UInventorySlot::Highlight()
{
	if (Background == nullptr) return;

	Background->SetColorAndOpacity(FLinearColor(0.17, 0.69, 1, 1));
}

void UInventorySlot::UnHighlight()
{
	if (Background == nullptr) return;

	Background->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
}


void UInventorySlot::Select()
{

}

void UInventorySlot::UnSelect()
{

}

