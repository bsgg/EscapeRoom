// Fill out your copyright notice in the Description page of Project Settings.


#include "PasswordPadlockUI.h"
#include "Game/GameLogic/Interactives/UI/CombinationUI.h"

bool UPasswordPadlockUI::Initialize()
{
	bool Success = Super::Initialize();

	return true;
}

void UPasswordPadlockUI::InitializeWidget(const FName& InCombination)
{
	Combination = InCombination.ToString();
	bIsLocked = true;
}

void UPasswordPadlockUI::OnShowWidget()
{
	Super::OnShowWidget();

	Combination_0->ResetCombination();
	Combination_1->ResetCombination();
	Combination_2->ResetCombination();
	Combination_3->ResetCombination();

	selectedCombinationSlot = 0;

	Combination_0->OnSelect();

	bIsLocked = false;
}

void UPasswordPadlockUI::Navigate(EDirectionType Direction)
{
	Super::Navigate(Direction);

	if ((Direction == EDirectionType::VE_DOWN) || (Direction == EDirectionType::VE_UP)) return;

	switch (selectedCombinationSlot)
	{
		case 0:
			Combination_0->OnDeselect();
		break;
		case 1:
			Combination_1->OnDeselect();
			break;
		case 2:
			Combination_2->OnDeselect();
			break;
		case 3:
			Combination_3->OnDeselect();
			break;
	}


	switch (Direction)
	{		
		break;
	case EDirectionType::VE_LEFT:
		selectedCombinationSlot--;
		if (selectedCombinationSlot < 0)
		{
			selectedCombinationSlot = 3;
		}
	break;
	case EDirectionType::VE_RIGHT:
		selectedCombinationSlot++;

		if (selectedCombinationSlot > 3)
		{
			selectedCombinationSlot = 0;
		}
	break;
	}

	switch (selectedCombinationSlot)
	{
	case 0:
		Combination_0->OnSelect();
		break;
	case 1:
		Combination_1->OnSelect();
		break;
	case 2:
		Combination_2->OnSelect();
		break;
	case 3:
		Combination_3->OnSelect();
		break;
	}

}

void UPasswordPadlockUI::OnFaceButtonPress(EFaceButtonType Button)
{
	// Test current password
	if (Button == EFaceButtonType::VE_BOTTOM) 
	{
		//CheckCombination();
		switch (selectedCombinationSlot)
		{
		case 0:
			Combination_0->OnChangeCharacterPress();
			break;
		case 1:
			Combination_1->OnChangeCharacterPress();
			break;
		case 2:
			Combination_2->OnChangeCharacterPress();
			break;
		case 3:
			Combination_3->OnChangeCharacterPress();
			break;
		}
	}

	if (Button == EFaceButtonType::VE_RIGHT)
	{
		CheckCombination();
	}

	Super::OnFaceButtonPress(Button);
}


void UPasswordPadlockUI::CheckCombination()
{
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

			GetWorld()->GetTimerManager().SetTimer(CompleteWidgetTimerHandler, this, &UPasswordPadlockUI::CompleteWidget, 3.0f, false, 3.0f);
		}
		else
		{
			int index = FMath::RandRange(0, IncorrectAnswerMessages.Num() - 1);
			MessageText->SetText(IncorrectAnswerMessages[index]);
		}
	}
}

void UPasswordPadlockUI::CompleteWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::CompleteWidget]"));

	bIsLocked = false;

	GetWorld()->GetTimerManager().ClearTimer(CompleteWidgetTimerHandler);

	Super::CompleteWidget();
}


