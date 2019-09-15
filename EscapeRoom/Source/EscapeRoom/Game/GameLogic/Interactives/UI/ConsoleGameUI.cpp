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
	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] %d"), 5);

	if ((!SlotMazeClass) || (!MazeGrid)) return;

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] SlotMazeClass and Maze grid ok"));

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

			if (IsPlayable) // if it's playable, shows only default color
			{
				
				NewSlot->SetSlotColor(DefaultColor);
			}
			else
			{
				if (value < SlotColors.Num())
				{
					NewSlot->SetSlotColor(SlotColors[value]);
				}
				else
				{
					NewSlot->SetSlotColor(DefaultColor);
				}
			}
				
			SlotContainer->AddChild(NewSlot);
			
		}
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


