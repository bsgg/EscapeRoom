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

	if (bRandomSequence)
	{
		// Set a random sequence of colors
		for (int i = 0; i < 8; i++)
		{
			int randomC = FMath::RandRange(0, 3);
			ColorSequence.Add(randomC);

			UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnShowWidget] randomC: %d"), randomC);
		}
	}

	bWait = true;

	ElpasedWait = 0.0f;
	WaitTime = WaitStartSimon;

	NextGamePhase = 0;
	   
	// Index and end starts the same
	IndexSequence = 0;
	IndexEndSequence = 0;

	bPlayerTurn = false;
	bLockInput = true;
	   	 
}

void USimonUI::HandleNextPhase()
{
	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::HandleNextPhase] NextGamePhase: %d"), NextGamePhase);

	if (NextGamePhase == 0) // AI Set Color
	{
		int indexColor = ColorSequence[IndexSequence];

		SetButtonToColor(indexColor, ButtonColors[indexColor]);

		// Set Next Phase to 1 (Set same button to default color) 
		NextGamePhase = 1;

		bWait = true;

		ElpasedWait = 0.0f;

		WaitTime = WaitButtonToDefaultColor; // Wait time to set button to default color
	}

	else if (NextGamePhase == 1) // AI Set same color to default
	{
		UE_LOG(LogTemp, Warning, TEXT("[USimonUI::HandleNextPhase] AI Phase Change Current Button to default"));

		SetButtonToColor(ColorSequence[IndexSequence], DefaultButtonColor);

		// Add  1 to sequence, and unlock players turn

		// Change Index Sequence
		IndexSequence += 1;

		if (IndexSequence > IndexEndSequence) // End Sequence
		{
			UE_LOG(LogTemp, Warning, TEXT("[USimonUI::HandleNextPhase] Player Turn %d/%d"), IndexSequence, IndexEndSequence);

			MessageText->SetText(FText::FromString("Your turn!"));

			IndexSequence = 0;

			NextGamePhase = -1;

			bPlayerTurn = true;

			bLockInput = false;
		}
		else // Not End Sequence
		{
			UE_LOG(LogTemp, Warning, TEXT("[USimonUI::HandleNextPhase] KEEP AI TURN %d/%d"), IndexSequence, IndexEndSequence);

			// Back to wait and set color
			NextGamePhase = 0;

			bWait = true;

			ElpasedWait = 0.0f;

			WaitTime = WaitButtonNextColor; // Wait time to set a new color

			bPlayerTurn = false;

			bLockInput = true;
		}
		
	}

	else if (NextGamePhase == 2) // Player Turn change button color to default and check current answer
	{
		UE_LOG(LogTemp, Warning, TEXT("[USimonUI::HandleNextPhase] Player change button pressed to default %i - %i "), ButtonPressedIndex, ColorSequence[IndexSequence]);

		SetButtonToColor(ButtonPressedIndex, DefaultButtonColor);

		NextGamePhase = -1;

		bPlayerTurn = false;

		bLockInput = true;

		if (ButtonPressedIndex == ColorSequence[IndexSequence]) // GOOD
		{
			// Check if reach end of current sequence, Check if change to AI is possible
			if (IndexSequence >= IndexEndSequence)
			{
				if (IndexEndSequence >= ColorSequence.Num() - 1) // Game completed
				{
					MessageText->SetText(FText::FromString("GOOD! YOU WIN CONGRATULATIONS!"));

					NextGamePhase = 3; // Set to completed the game

					bWait = true;

					WaitTime = WaitGameCompleted;

					ElpasedWait = 0.0f;

					bWait = true;
				}
				else
				{
					// Add 1 to IndexEndSequence
					MessageText->SetText(FText::FromString("GOOD! YOU FINISED YOUR TURN, LET ME CONTINUE"));

					IndexEndSequence += 1;

					IndexSequence = 0; // Resets sequence

					NextGamePhase = 0; // Set turn to AI

					bWait = true;

					ElpasedWait = 0.0f;

					WaitTime = WaitSwitchTurn;

				}
			}
			else
			{
				// Add 1 to next sequence
				MessageText->SetText(FText::FromString("GOOD! KEEP GOING "));
				IndexSequence++;

				bPlayerTurn = true;
				bLockInput = false;
			}
		}
		else
		{
			MessageText->SetText(FText::FromString("INCORRECT, TRY AGAIN"));

			// TODO: RESET SEQUENCE AND START OVER
			IndexSequence = 0;
			IndexEndSequence = 0;

			bWait = true;

			ElpasedWait = 0.0f;
			WaitTime = WaitStartSimon;

			NextGamePhase = 0;
		}

	}

	else if (NextGamePhase == 3) // Player Turn change button color to default and check current answer
	{
		UE_LOG(LogTemp, Warning, TEXT("[USimonUI::HandleNextPhase] END OF SIMON "));

		MessageText->SetText(FText::FromString("ALL GOOD YOU CAN CONTINUE"));

		if (UIInterface != nullptr)
		{
			UIInterface->OnComplete();
		}
	}
}

void USimonUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (bWait)
	{
		ElpasedWait += DeltaTime;
		if (ElpasedWait >= WaitTime)
		{
			bWait = false;
			HandleNextPhase();
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

	ButtonPressedIndex = indexButton;

	bLockInput = true;

	// Set wait
	bWait = true;

	ElpasedWait = 0.0f;

	WaitTime = WaitButtonToDefaultColor; // Set button to default

	// Phase Change color to default
	NextGamePhase = 2;
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
