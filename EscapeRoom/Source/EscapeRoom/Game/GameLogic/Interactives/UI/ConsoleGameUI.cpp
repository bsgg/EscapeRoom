// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleGameUI.h"

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

}

void UConsoleGameUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

}


