// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveUIComponent.h"
#include "InteractableUI.h"
#include "Game/UI/ControlWidget.h"
#include "UnrealNetwork.h"

UInteractiveUIComponent::UInteractiveUIComponent()
{
	SetIsReplicated(true);
}

void UInteractiveUIComponent::BeginPlay()
{
	Super::BeginPlay();

	UI = Cast<UInteractableUI>(this->GetUserWidgetObject());
	
}

void UInteractiveUIComponent::Initialize()
{
	if (UI == nullptr) return;
	UI->GetAControl()->Hide();
	UI->GetXControl()->Hide();
}
