// Fill out your copyright notice in the Description page of Project Settings.


#include "UIBasicInteractive.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUIBasicInteractive::SetInteraface(IUIBasicInteractiveInterface* UIBasicInteractiveInterface)
{
	this->UIInterface = UIBasicInteractiveInterface;
}

bool UUIBasicInteractive::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (EnterCombinationButton == nullptr) return false;
	EnterCombinationButton->OnClicked.AddDynamic(this, &UUIBasicInteractive::OnEnterCombinationPressed);

	if (ExitButton == nullptr) return false;
	ExitButton->OnClicked.AddDynamic(this, &UUIBasicInteractive::OnExitPressed);

	if (MessageText == nullptr) return false;
	MessageText->SetText(FText::FromString(""));

	return true;
}

void UUIBasicInteractive::InitializeWidget(const FName& Combination)
{
	UE_LOG(LogTemp, Warning, TEXT("[UUIBasicInteractive::InitializeWidget] Combination: %s"), *Combination.ToString());
}

void UUIBasicInteractive::OnEnterCombinationPressed()
{
	// TODO CHECK COMBINATION
}

void UUIBasicInteractive::OnExitPressed()
{
	if (UIInterface != nullptr)
	{
		UIInterface->ExitUI();
	}
}

