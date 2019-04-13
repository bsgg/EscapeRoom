// Fill out your copyright notice in the Description page of Project Settings.


#include "UIBasicInteractive.h"
#include "Components/Button.h"

void UUIBasicInteractive::SetInteraface(IUIBasicInteractiveInterface* UIBasicInteractiveInterface)
{
	this->UIBasicInteractiveInterface = UIBasicInteractiveInterface;
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
	if (UIBasicInteractiveInterface != nullptr)
	{
		UIBasicInteractiveInterface->ExitUI();
	}
}

