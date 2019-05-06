// Fill out your copyright notice in the Description page of Project Settings.


#include "UIBasicInteractive.h"
#include "Game/GameLogic/Interactives/UI/CombinationUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

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
	MessageText->SetText(InitialMessages);

	bIsLocked = false;

	return true;
}

void UUIBasicInteractive::InitializeWidget(const FName& InCombination)
{
	Combination = InCombination.ToString();
}

void UUIBasicInteractive::OnShowWidget()
{

}

void UUIBasicInteractive::OnEnterCombinationPressed()
{	
	if (bIsLocked) return;

	if (Combination.Len() >= 4)
	{
		int CorrectChars = 0;

		if (Combination[0] == Combination_0->GetCharacter()[0])
		{
			CorrectChars += 1;
		}

		if (Combination[1] == Combination_1->GetCharacter()[0])
		{
			CorrectChars += 1;
		}

		if (Combination[2] == Combination_2->GetCharacter()[0])
		{
			CorrectChars += 1;
		}

		if (Combination[3] == Combination_3->GetCharacter()[0])
		{
			CorrectChars += 1;
		}

		if (CorrectChars >= 4)
		{	
			bIsLocked = true;

			int index = FMath::RandRange(0, CorrectAnswerMessages.Num() - 1);
			MessageText->SetText(CorrectAnswerMessages[index]);

			GetWorld()->GetTimerManager().SetTimer(CompleteWidgetTimerHandler, this, &UUIBasicInteractive::CompleteWidget, 3.0f, false, 3.0f);
		}
		else
		{
			int index = FMath::RandRange(0, IncorrectAnswerMessages.Num() - 1);
			MessageText->SetText(IncorrectAnswerMessages[index]);
		}
	}
}
void UUIBasicInteractive::CompleteWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::CompleteWidget]"));

	bIsLocked = false;

	GetWorld()->GetTimerManager().ClearTimer(CompleteWidgetTimerHandler);

	if (UIInterface != nullptr)
	{
		UIInterface->OnComplete();
	}
}


void UUIBasicInteractive::OnExitPressed()
{
	if (bIsLocked) return;

	bIsLocked = false;

	if (UIInterface != nullptr)
	{
		UIInterface->ExitUI();
	}
}

