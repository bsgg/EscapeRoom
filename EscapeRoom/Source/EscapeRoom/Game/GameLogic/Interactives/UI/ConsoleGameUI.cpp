// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleGameUI.h"
#include "Components/SizeBox.h"

bool UConsoleGameUI::Initialize()
{
	bool Success = Super::Initialize();


	return true;

}

void UConsoleGameUI::InitializeWidget(const FName& Combination)
{
	Super::InitializeWidget(Combination);


	
	
}

void UConsoleGameUI::OnShowWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] %d"), 5);

	if ((!SlotMazeClass) || (!MazeGrid)) return;

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] SlotMazeClass and Maze grid ok"));

	USlotMaze* NewSlot = CreateWidget<USlotMaze>(GetWorld(), SlotMazeClass);
	if (!NewSlot) return;

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget]  New slot created"));

	NewSlot->SetSlotColor(DefaultSlotColor);

	// Get first child in grid
	USizeBox* SlotContainer = Cast<USizeBox>(MazeGrid->GetChildAt(0));
	if (!SlotContainer) return;

	UE_LOG(LogTemp, Warning, TEXT("[UConsoleGameUI::OnShowWidget] SlotContainer created"));

	SlotContainer->AddChild(NewSlot);
	
}

void UConsoleGameUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

}


