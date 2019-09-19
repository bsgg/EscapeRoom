// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleGameUI.h"
#include "Components/SizeBox.h"
#include "EscapeRoomGameModeBase.h"
#include "EscapeRoomGameInstance.h"

bool UConsoleGameUI::Initialize()
{
	bool Success = Super::Initialize();


	return true;

}

void UConsoleGameUI::InitializeWidget(const FName& Combination)
{
	Super::InitializeWidget(Combination);

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::InitializeWidget] Combination: %s"), *Combination.ToString());

	TArray<FString> Rows;
	Combination.ToString().ParseIntoArray(Rows, TEXT("|"), true);

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::InitializeWidget] Number Rows %i"), Rows.Num());
	
	for (int y = 0; y < Rows.Num(); y++)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::InitializeWidget] Rows [%i] = %s"), y, *Rows[y]);

		TArray<FString> Columns;
		Rows[y].ParseIntoArray(Columns, TEXT(","), true);

		UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::InitializeWidget] Number Columns %i"), Columns.Num());
		for (int x = 0; x < Columns.Num(); x++)
		{
			int32 value = FCString::Atoi(*Columns[x]);

			UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::InitializeWidget] Columns [%i] = %s -> %i"), x, *Columns[x], value);

			maze[x][y] = value;
		}

	}
}

void UConsoleGameUI::OnShowWidget()
{
	if ((!SlotMazeClass) || (!MazeGrid)) return;

	int i = 0;
	for (int x = 0; x < GridColumns; x++)
	{
		for (int y = 0; y < GridRows; y++)
		{
			USlotMaze* NewSlot = CreateWidget<USlotMaze>(GetWorld(), SlotMazeClass);

			USizeBox* SlotContainer = Cast<USizeBox>(MazeGrid->GetChildAt(i));

			i++;

			if ((!NewSlot)|| (!SlotContainer)) continue;

			// Pull value
			int value = maze[x][y];

			NewSlot->SetSlotHighlightColor(UnHighlightColor);

			NewSlot->SetSlotColor(DefaultColor);

			NewSlot->SetIndex(-1);

			if (!IsPlayable) // if it's playable, shows only default color
			{				
				
				if (value < SlotColors.Num())
				{

					NewSlot->SetSlotColor(SlotColors[value]);

					NewSlot->SetIndex(value);
				}				
			}
			
			SlotContainer->AddChild(NewSlot);

			SlotMazeList.Add(NewSlot);
			
		}
	}

	selectedRowButton = 0;

	selectedColumnButton = 0;

	selectedIndex = 0;

	if (SlotMazeList.Num() > 0)
	{
		SlotMazeList[selectedIndex]->SetSlotHighlightColor(HighlightColor);
	}

	/*AEscapeRoomGameModeBase* GameMode = (AEscapeRoomGameModeBase*)GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] GameMode Exits"));

		UEscapeRoomGameInstance* GameInstance = Cast<UEscapeRoomGameInstance>(GameMode->GetGameInstance());
		if (GameInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] GameInstance Exitsts"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] GameInstance NO Exitsts"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] GameMode NO Exits"));
	}*/
}

void UConsoleGameUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

}

void UConsoleGameUI::Navigate(EDirectionType Direction)
{
	if (!IsPlayable) return;

	Super::Navigate(Direction);

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::Navigate] %d"), Direction);

	switch (Direction)
	{
		case EDirectionType::VE_DOWN:
			selectedRowButton += 1;

			if (selectedRowButton >= GridRows)
			{
				selectedRowButton = selectedRowButton - 1;
			}
		break;
		case EDirectionType::VE_UP:
			selectedRowButton -= 1;

			if (selectedRowButton < 0)
			{
				selectedRowButton = 0;
			}
		break;
		case EDirectionType::VE_LEFT:
			selectedColumnButton -= 1;

			if (selectedColumnButton < 0)
			{
				selectedColumnButton = 0;
			}
		break;
		case EDirectionType::VE_RIGHT:
			selectedColumnButton += 1;

			if (selectedColumnButton >= GridColumns)
			{
				selectedColumnButton = GridColumns - 1;
			}
		break;

	}

	int32 currentIndex = selectedRowButton * GridRows + selectedColumnButton;

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::Navigate] selectedIndex: %d -  currentIndex %d/ %d"), selectedIndex,  currentIndex , SlotMazeList.Num());
	if (currentIndex < SlotMazeList.Num())
	{
		// Highlight
		SlotMazeList[currentIndex]->SetSlotHighlightColor(HighlightColor);

		if (selectedIndex > -1)
		{
			// UnHighlight
			SlotMazeList[selectedIndex]->SetSlotHighlightColor(UnHighlightColor);
		}
	
		selectedIndex = currentIndex;
	}
}

void UConsoleGameUI::OnFaceButtonPress(EFaceButtonType Button)
{
	Super::OnFaceButtonPress(Button);

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnFaceButtonPress] Button: %d "), Button);

	if (Button == EFaceButtonType::VE_BOTTOM) // Change color
	{		
		UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnFaceButtonPress] Change color in slot")); 

		if (selectedIndex > -1)
		{

			int32 currentIndexColor = SlotMazeList[selectedIndex]->GetIndex();

			currentIndexColor++;

			if (currentIndexColor >= SlotColors.Num())
			{
				currentIndexColor = 0;
			}

			// Set new color
			SlotMazeList[selectedIndex]->SetIndex(currentIndexColor);

			SlotMazeList[selectedIndex]->SetSlotColor(SlotColors[currentIndexColor]);

		}
	}

	if (Button == EFaceButtonType::VE_RIGHT) // Check combination
	{
		UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnFaceButtonPress] Check solution"));
	}

	if (Button == EFaceButtonType::VE_TOP) // Exit
	{
		UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnFaceButtonPress] Exit UI"));

		if (UIInterface != nullptr)
		{
			UIInterface->ExitUI();
		}
	}
}


