// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicInspectBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"

void ABasicInspectBI::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

	if (CurrentController == nullptr) return;

	CurrentController->ShowMessage(InspectMessage, 2.0f);

	if (CharacterOverlapping != nullptr)
	{
		CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
	}

	// Show widget if exists		
	if (HasWidget())
	{
		GetWorld()->GetTimerManager().SetTimer(WidgetTimerHandle, this, &ABasicInspectBI::OnShowWidget, 1.5f, false);
	}

}


void ABasicInspectBI::OnShowWidget()
{
	GetWorld()->GetTimerManager().ClearTimer(WidgetTimerHandle);
	ShowWidget();
}

