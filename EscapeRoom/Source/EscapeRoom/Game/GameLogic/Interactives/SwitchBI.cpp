// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"

void ASwitchBI::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

	if (CurrentController == nullptr) return;

	if (IsSwitchOn)
	{
		CurrentController->ShowMessage(SwitchOffDetail.ToString(), 2.0f);
	}
	else
	{
		
		CurrentController->ShowMessage(SwitchOnDetail.ToString(), 2.0f);
	}

	if (CharacterOverlapping != nullptr)
	{
		CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
	}

	if (Role < ROLE_Authority)
	{
		ServerDoToggleAction();
	}
	else
	{
		DoToggleAction();
	}
}


void ASwitchBI::ServerDoToggleAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[ASwitchBI::ServerDoToggleAction]"));

	DoToggleAction();
}

bool ASwitchBI::ServerDoToggleAction_Validate()
{
	return true;
}

void ASwitchBI::DoToggleAction()
{
	PlayInteractSound();

	if (IsSwitchOn)
	{
		IsSwitchOn = false;
	}
	else
	{
		IsSwitchOn = true;
	}	
	
	ABasicInteractive* interactive = FindConnectedInteractive();

	if (interactive != nullptr)
	{
		interactive->InteractOnConnectedInteractive();
		UE_LOG(LogTemp, Warning, TEXT("[ASwitchBI::DoToggleAction] Connected Interactive found"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] Connected Interactive not found "));
	}
}


void ASwitchBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);	

	DOREPLIFETIME(ASwitchBI, IsSwitchOn);
}