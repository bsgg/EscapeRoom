
#include "CombinationUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


bool UCombinationUI::Initialize()
{
	bool Success = Super::Initialize();

	ABCArray = { "0", "1", "2", "3","4","5","6","7","8","9", "A",  "B",  "C",  "D",  "E", "F",  "G",  "H", "I", "J", "K", "L", "M", "O", "P", "Q", "R", "S", "T", "U", "V", "W",  "X",  "Y", "Z" };

	if (UpButton == nullptr) return false;
	UpButton->OnClicked.AddDynamic(this, &UCombinationUI::OnUpButtonPressed);

	if (DownButton == nullptr) return false;
	DownButton->OnClicked.AddDynamic(this, &UCombinationUI::OnDownButtonPressed);

	ResetCombination();	

	CharacterText->SetText(FText::FromString(ABCArray[ChainIndex]));

	return true;
}

void UCombinationUI::Locked()
{
	bIsLocked = true;
}

void UCombinationUI::Unlocked()
{
	bIsLocked = false;
}

void UCombinationUI::OnUpButtonPressed()
{
	if (bIsLocked) return;

	ChainIndex += 1;

	if (ChainIndex >= ABCArray.Num())
	{
		ChainIndex = 0;
	}

	if (CharacterText == nullptr) return;
	CharacterText->SetText(FText::FromString(ABCArray[ChainIndex]));
}

void UCombinationUI::OnDownButtonPressed()
{
	if (bIsLocked) return;

	ChainIndex -= 1;

	if (ChainIndex < 0)
	{
		ChainIndex = ABCArray.Num() -1;
	}

	if (CharacterText == nullptr) return;
	CharacterText->SetText(FText::FromString(ABCArray[ChainIndex]));
}


void UCombinationUI::ResetCombination()
{
	if (CharacterText == nullptr) return;

	CharacterText->SetText(FText::FromString(""));
	ChainIndex = 0;
}

FString UCombinationUI::GetCharacter() const
{
	if (CharacterText == nullptr) return "";

	return CharacterText->Text.ToString();
}