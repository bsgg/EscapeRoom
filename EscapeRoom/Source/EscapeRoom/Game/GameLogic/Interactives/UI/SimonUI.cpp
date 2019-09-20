// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/GameLogic/Interactives/UI/SlotMaze.h"
#include "Kismet/GameplayStatics.h"

bool USimonUI::Initialize()
{
	bool Success = Super::Initialize();	

	Slots.Add(Slot_0);
	Slots.Add(Slot_1);
	Slots.Add(Slot_2);
	Slots.Add(Slot_3);
	Slots.Add(Slot_4);

	return true;
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


void USimonUI::ResetSlots()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetSlotColor(DefaultButtonColor);
		Slots[i]->SetSlotHighlightColor(UnHighlightColor);
	}
}
	

void USimonUI::OnShowWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnShowWidget] %d"), ColorSequence.Num());

	MessageText->SetText(FText::FromString("Follow the sequence"));

	if (bRandomSequence)
	{
		ColorSequence.Empty();

		// Set a random sequence of colors
		for (int i = 0; i < 8; i++)
		{
			int randomC = FMath::RandRange(0, 4);
			ColorSequence.Add(randomC);

			UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnShowWidget] randomC: %d"), randomC);
		}
	}

	ResetSlots();

	bWait = true;

	ButtonPressedIndex = 0;

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

		SetSlotToColor(indexColor, ButtonColors[indexColor]);

		if (indexColor < Sounds.Num())
		{
			UGameplayStatics::PlaySound2D(this, Sounds[indexColor]);
		}

		// Set Next Phase to 1 (Set same button to default color) 
		NextGamePhase = 1;

		bWait = true;

		ElpasedWait = 0.0f;

		WaitTime = WaitButtonToDefaultColor; // Wait time to set button to default color
	}

	else if (NextGamePhase == 1) // AI Set same color to default
	{
		UE_LOG(LogTemp, Warning, TEXT("[USimonUI::HandleNextPhase] AI Phase Change Current Button to default"));

		SetSlotToColor(ColorSequence[IndexSequence], DefaultButtonColor);

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

			ButtonPressedIndex = 0;

			Slots[ButtonPressedIndex]->SetSlotHighlightColor(HighlightColor);


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

	else if (NextGamePhase == 2) // PLAYER TURN: Change slot color to default and check current answer
	{
		UE_LOG(LogTemp, Warning, TEXT("[USimonUI::HandleNextPhase] Player change button pressed to default %i - %i "), ButtonPressedIndex, ColorSequence[IndexSequence]);

		SetSlotToColor(ButtonPressedIndex, DefaultButtonColor);

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

					ButtonPressedIndex = 0;
					ResetSlots();

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

			for (int i = 0; i < Slots.Num(); i++)
			{
				Slots[i]->SetSlotColor(DefaultButtonColor);
				Slots[i]->SetSlotHighlightColor(UnHighlightColor);
			}

			ButtonPressedIndex = 0;

			ResetSlots();

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



void USimonUI::SetSlotToColor(int indexButton, FLinearColor Color)
{
	if ((indexButton >= 0) && (indexButton < Slots.Num()))
	{
		Slots[indexButton]->SetSlotColor(Color);
	}
}

void USimonUI::Navigate(EDirectionType Direction)
{
	if (bLockInput) return;

	Super::Navigate(Direction);

	if ((Direction == EDirectionType::VE_DOWN) || (Direction == EDirectionType::VE_UP)) return;

	Slots[ButtonPressedIndex]->SetSlotHighlightColor(UnHighlightColor);

	switch (Direction)
	{
		break;
	case EDirectionType::VE_LEFT:
		ButtonPressedIndex--;
		if (ButtonPressedIndex < 0)
		{
			ButtonPressedIndex = Slots.Num()-1;
		}
		break;
	case EDirectionType::VE_RIGHT:
		ButtonPressedIndex++;

		if (ButtonPressedIndex >= Slots.Num())
		{
			ButtonPressedIndex = 0;
		}
		break;
	}

	Slots[ButtonPressedIndex]->SetSlotHighlightColor(HighlightColor);

}

void USimonUI::OnFaceButtonPress(EFaceButtonType Button)
{
	// Test current password
	if (Button == EFaceButtonType::VE_BOTTOM)
	{
		if (bLockInput) return;

		UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnFaceButtonPress]"));

		SetSlotToColor(ButtonPressedIndex, ButtonColors[ButtonPressedIndex]);

		if (ButtonPressedIndex < Sounds.Num())
		{
			UGameplayStatics::PlaySound2D(this, Sounds[ButtonPressedIndex]);
		}

		bLockInput = true;

		// Set wait
		bWait = true;

		ElpasedWait = 0.0f;

		WaitTime = WaitButtonToDefaultColor; // Set button to default

		// Phase Change color to default
		NextGamePhase = 2;
	}

	Super::OnFaceButtonPress(Button);

}

