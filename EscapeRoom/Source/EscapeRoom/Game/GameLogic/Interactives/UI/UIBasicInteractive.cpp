// Fill out your copyright notice in the Description page of Project Settings.


#include "UIBasicInteractive.h"
#include "Game/GameLogic/Interactives/UI/CombinationUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UUIBasicInteractive::SetInteraface(IUIBasicInteractiveInterface* UIBasicInteractiveInterface)
{
	this->UIInterface = UIBasicInteractiveInterface;
}

bool UUIBasicInteractive::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (MessageText == nullptr) return false;
	MessageText->SetText(InitialMessages);	

	return true;
}

void UUIBasicInteractive::InitializeWidget(const FName& InCombination)
{
	
}

void UUIBasicInteractive::OnShowWidget()
{

}

void UUIBasicInteractive::Navigate(EDirectionType Direction)
{
	//UE_LOG(LogTemp, Warning, TEXT("[UUIBasicInteractive::Navigate]"));
}

void UUIBasicInteractive::OnFaceButtonPress(EFaceButtonType Button)
{
	if (Button == EFaceButtonType::VE_TOP) // Exit
	{
		UE_LOG(LogTemp, Warning, TEXT("[UUIBasicInteractive::OnFaceButtonPress] EFaceButtonType::VE_TOP"));
		ExitWidget();
	}
}


void UUIBasicInteractive::CompleteWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[UUIBasicInteractive::CompleteWidget]"));

	if (UIInterface != nullptr)
	{
		UIInterface->OnComplete();
	}
}

void UUIBasicInteractive::ExitWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[UUIBasicInteractive::ExitWidget]"));

	if (UIInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UUIBasicInteractive::ExitWidget] UIInterface not null"));

		UIInterface->ExitUI();
	}
}

//void UUIBasicInteractive::CompleteWidget()
//{
	//UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::CompleteWidget]"));

	//bIsLocked = false;

	//GetWorld()->GetTimerManager().ClearTimer(CompleteWidgetTimerHandler);

	//if (UIInterface != nullptr)
	//{
		//UIInterface->OnComplete();
	//}
//}


/*void UUIBasicInteractive::OnExitPressed()
{
	if (bIsLocked) return;

	bIsLocked = false;

	if (UIInterface != nullptr)
	{
		UIInterface->ExitUI();
	}
}*/

