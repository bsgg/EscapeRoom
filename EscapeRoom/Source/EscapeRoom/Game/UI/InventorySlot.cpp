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

	Hide();

	return true;
}

void UInventorySlot::Show()
{
	bIsEmpty = false;
	SlotBox->SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlot::Hide()
{
	bIsEmpty = true;
	SlotBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::SetObjectSlot(const FObjectInteraction& Object)
{
	ObjectSlot = Object;

	SetImageSlot(ObjectSlot.Thumbnail);

	SetTitleSlot(ObjectSlot.Name);
}


void UInventorySlot::SetImageSlot(UTexture2D* Image)
{
	if ((Thumbnail == nullptr) || (Image == nullptr)) return;

	Thumbnail->SetBrushFromTexture(Image);
}

void UInventorySlot::SetTitleSlot(FText Title)
{
	if (TitleText == nullptr) return;

	TitleText->SetText(Title);
}