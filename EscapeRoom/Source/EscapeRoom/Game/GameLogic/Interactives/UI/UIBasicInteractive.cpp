// Fill out your copyright notice in the Description page of Project Settings.


#include "UIBasicInteractive.h"
#include "Components/Button.h"

void UUIBasicInteractive::SetInteraface(IUIBasicInteractiveInterface* UIInterface)
{
	this->UIInterface = UIInterface;
}

bool UUIBasicInteractive::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (ExitButton == nullptr) return false;
	ExitButton->OnClicked.AddDynamic(this, &UUIBasicInteractive::OnExitPressed);

	return true;
}

void UUIBasicInteractive::OnExitPressed()
{
	if (UIInterface != nullptr)
	{
		UIInterface->ExitUI();
	}
}

