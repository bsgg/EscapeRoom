// Fill out your copyright notice in the Description page of Project Settings.


#include "UseWithUIBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"


AUseWithUIBI::AUseWithUIBI()
{
	
}

void AUseWithUIBI::BeginPlay()
{
	Super::BeginPlay();

}

void AUseWithUIBI::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

	if (CurrentController == nullptr) return;

	CurrentController->CreateInteractiveUI(ObjectID);

	if (CurrentController->GetInteractiveUI() != nullptr)
	{
		CurrentController->GetInteractiveUI()->SetInteraface(this);
	}

	if (CharacterOverlapping != nullptr)
	{
		CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
	}
	
}

void AUseWithUIBI::ExitUI()
{
	if (CurrentController == nullptr) return;

	CurrentController->RemoveInteractiveUI();
}