// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonUI.h"
#include "Components/Button.h"

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

	return true;
}

void USimonUI::InitializeWidget(const FName& Combination)
{
	Super::InitializeWidget(Combination);

	if (ColorSequence.Num() >= 4)
	{
		ColorButton_1->SetBackgroundColor(ColorSequence[0]);
		ColorButton_2->SetBackgroundColor(ColorSequence[1]);
		ColorButton_3->SetBackgroundColor(ColorSequence[2]);
		ColorButton_4->SetBackgroundColor(ColorSequence[3]);
	}

}

void USimonUI::OnShowWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnShowWidget] %d"), ColorSequence.Num());

	
}

void USimonUI::OnColorButton_1Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnColorButton_1Pressed]"));
}

void USimonUI::OnColorButton_2Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnColorButton_2Pressed]"));
}

void USimonUI::OnColorButton_3Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnColorButton_3Pressed]"));
}

void USimonUI::OnColorButton_4Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("[USimonUI::OnColorButton_4Pressed]"));
}
