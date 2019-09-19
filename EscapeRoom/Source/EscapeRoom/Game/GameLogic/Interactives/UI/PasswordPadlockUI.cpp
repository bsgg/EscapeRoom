// Fill out your copyright notice in the Description page of Project Settings.


#include "PasswordPadlockUI.h"

bool UPasswordPadlockUI::Initialize()
{
	bool Success = Super::Initialize();

	return true;
}

void UPasswordPadlockUI::InitializeWidget(const FName& InCombination)
{
	Combination = InCombination.ToString();
	bIsLocked = false;
}

void UPasswordPadlockUI::Navigate(EDirectionType Direction)
{
	Super::Navigate(Direction);

	switch (Direction)
	{
	case EDirectionType::VE_DOWN:
		
	break;
	case EDirectionType::VE_UP:
		
		break;
	case EDirectionType::VE_LEFT:
		
	break;
	case EDirectionType::VE_RIGHT:
		
	break;
	}

}

void UPasswordPadlockUI::OnFaceButtonPress(EFaceButtonType Button)
{
	// Test current password
	if (Button == EFaceButtonType::VE_BOTTOM) 
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


