// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot.h"
#include "Components/Image.h"

bool UInventorySlot::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	bIsEmpty = true;
	Thumbnail->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UInventorySlot::SetImageSlot(UTexture2D* Image)
{
	if (Thumbnail == nullptr) return;

	if (Image == nullptr)
	{
		bIsEmpty = true;
		Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		bIsEmpty = false;

		Thumbnail->SetVisibility(ESlateVisibility::Visible);
		Thumbnail->SetBrushFromTexture(Image);		
	}	
}