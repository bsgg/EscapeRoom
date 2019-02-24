// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableUI.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"

bool UInteractableUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;


	return true;
}


