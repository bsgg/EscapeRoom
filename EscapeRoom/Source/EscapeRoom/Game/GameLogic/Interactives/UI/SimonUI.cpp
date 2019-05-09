// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool USimonUI::Initialize()
{
	bool Success = Super::Initialize();	

	if (ColorButton_1 == nullptr) return false;
	ColorButton_1->OnClicked.AddDynamic(this, &USimonUI::OnColorButton_1Pressed);

	if (ColorButton_2 == nullptr) return false;
	ColorButton_2->OnClicked.AddDynamic(this, &USimonUI::OnColorButton_2Pressed);

	if (ColorButton_3 == nullptr) return false;
	ColorButton_3->OnClicked.AddDynamic(this, &USimonUI::OnColorButton_3Pressed);

	if (ColorButton_4 == nullptr) return false;
	ColorButton_4->OnClicked.AddDynamic(this, &USimonUI::OnColorButton_4Pressed);

	ColorButtons.Add(ColorButton_1);
	ColorButtons.Add(ColorButton_2);
	ColorButtons.Add(ColorButton_3);
	ColorButtons.Add(ColorButton_4);

	return true;
}

void USimonUI::InitializeWidget(const FName& Combination)
{
	Super::InitializeWidget(Combination);

	// Set buttons to default color
	for (int i = 0; i < ColorButtons.Num(); i++)
	{
		ColorButtons[i]->SetBackgroundColor(DefaultButtonColor);
	}
}
	

void USimonUI::OnShowWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnShowWidget] %d"), ColorSequence.Num());

	MessageText->SetText(FText::FromString("Follow the sequence"));

	// Set a random sequence of colors
	for (int i = 0; i < 8; i++)
	{
		int randomC = FMath::RandRange(0, 3);
		ColorSequence.Add(randomC);

		UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnShowWidget] randomC: %d"), randomC);
	}

	bLockInput = true;

	//PreIndexSequence = -1;

	IndexSequence = 0;

	IndexEndSequence = 1;

	bWaitToSetColor = true;

	bWaitToSetDefaultColor = false;

	bPlayerTurn = false;

}

void USimonUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	// Set to different color
	if (bWaitToSetColor)
	{
		ElpasedToSetColor += DeltaTime;

		if (ElpasedToSetColor >= WaitTimeToSetColor)
		{
			ElpasedToSetColor = 0.0f;

			bWaitToSetColor = false;

			int indexColor = ColorSequence[IndexSequence];

			UE_LOG(LogTemp, Warning, TEXT("[USimonUI::NativeTick] indexColor: %d IndexSequence: %d"), indexColor, IndexSequence);

			SetButtonToColor(indexColor, ButtonColors[indexColor]);

			// Wait to set button to default color
			bWaitToSetDefaultColor = true;
		}
	}

	if (bWaitToSetDefaultColor)
	{
		ElpasedToSetColor += DeltaTime;

		if (ElpasedToSetColor >= WaitTimeToSetColor)
		{
			ElpasedToSetColor = 0.0f;

			bWaitToSetDefaultColor = false;

			

			// Check if player Turn
			if (bPlayerTurn)
			{
				SetButtonToColor(ButtonPressedIndex, DefaultButtonColor);

				// Check if button pressed correct
				if (ButtonPressedIndex == ColorSequence[IndexSequence])
				{
					MessageText->SetText(FText::FromString("GOOD!"));

					// TODO: Add 1 to index sequence
				}
				else
				{
					MessageText->SetText(FText::FromString("INCORRECT!!"));

					// TODO: RESET SEQUENCE AND START OVER
				}
			}
			else
			{
				SetButtonToColor(IndexSequence, DefaultButtonColor);

				// Change Index Sequence
				IndexSequence += 1;

				if (IndexSequence >= IndexEndSequence)
				{
					// Wait for user to answer

					bLockInput = false;

					MessageText->SetText(FText::FromString("Answer now"));

					IndexSequence = 0;

					bPlayerTurn = true;
				}
			}
		}
	}

}

void USimonUI::SetButtonToColor(int indexButton, FLinearColor Color)
{
	if ((indexButton >= 0) && (indexButton < ButtonColors.Num()))
	{
		ColorButtons[indexButton]->SetBackgroundColor(Color);
	}
}


void USimonUI::OnButtonPressed(int indexButton)
{
	SetButtonToColor(indexButton, ButtonColors[indexButton]);

	bLockInput = true;

	ElpasedToSetColor = 0.0f;

	// CHECK ANSWER
	ButtonPressedIndex = indexButton;
	
	bWaitToSetDefaultColor = true;

}



void USimonUI::OnColorButton_1Pressed()
{
	if (bLockInput) return;

	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnColorButton_1Pressed]"));

	OnButtonPressed(0);	
}


void USimonUI::OnColorButton_2Pressed()
{
	if (bLockInput) return;

	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnColorButton_2Pressed]"));

	OnButtonPressed(1);
}

void USimonUI::OnColorButton_3Pressed()
{
	if (bLockInput) return;

	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnColorButton_3Pressed]"));

	OnButtonPressed(2);
}

void USimonUI::OnColorButton_4Pressed()
{
	if (bLockInput) return;

	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnColorButton_4Pressed]"));

	OnButtonPressed(3);
}
