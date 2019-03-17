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
	SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlot::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);}


void UInventorySlot::SetObjectSlot(const FObjectInteraction& Object)
{
	ObjectSlot = Object;

	SetImageSlot(ObjectSlot.Thumbnail);

	bIsEmpty = false;
}

void UInventorySlot::SetToDefault()
{
	if (Thumbnail == nullptr) return;

	Thumbnail->SetVisibility(ESlateVisibility::Hidden);

	bIsEmpty = true;
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

	Thumbnail->SetColorAndOpacity(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));
}

void UInventorySlot::UnHighlight()
{
	if (Thumbnail == nullptr) return;

	Thumbnail->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}


