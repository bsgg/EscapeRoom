// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
#include "Game/RoomGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"

void ASwitchBI::StartInteract(APawn* Instigator)
{
	Super::StartInteract(Instigator);

	if (CurrentController == nullptr) return;

	if (IsSwitchOn)
	{
		CurrentController->ShowMessage(SwitchOnDetail.ToString(), 2.0f);
	}
	else
	{
		CurrentController->ShowMessage(SwitchOffDetail.ToString(), 2.0f);
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
	if (OverlappedSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, OverlappedSound);
	}

	if (IsSwitchOn)
	{
		IsSwitchOn = false;
	}
	else
	{
		IsSwitchOn = true;
	}

	ARoomGameMode* GM = Cast<ARoomGameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr) return;
	
	UE_LOG(LogTemp, Warning, TEXT("[ASwitchBI::DoToggleAction] ARoomGameMode NOT NULL "));
	ABasicInteractive* interactive = GM->FindInteractiveById(ConnectedInteractiveID);

	if (interactive != nullptr)
	{
		//AToggleInteractive* ToggleInteractive = Cast<AToggleInteractive>(interactive);

		//if (ToggleInteractive)
		//{
			//UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] Connected Interactive found %s "), *ToggleInteractive->GetData().ID.ToString());

			//ToggleInteractive->Toggle();
		//}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] Connected Interactive not found "));
	}
}


void ASwitchBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASwitchBI, ConnectedInteractiveID);

	DOREPLIFETIME(ASwitchBI, IsSwitchOn);
}