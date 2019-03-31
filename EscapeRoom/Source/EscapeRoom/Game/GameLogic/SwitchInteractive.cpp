// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchInteractive.h"
#include "Game/RoomGameMode.h"
#include "Game/GameLogic/ToggleInteractive.h"
#include "UnrealNetwork.h"


void ASwitchInteractive::Toggle()
{
	UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] %s "), *ConnectedInteractiveID.ToString());

	ARoomGameMode* GM = Cast<ARoomGameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr) return;

	/*AInteractiveBase* interactive = GM->FindInteractiveById(ConnectedInteractiveID);

	if (interactive != nullptr)
	{
		AToggleInteractive* ToggleInteractive = Cast<AToggleInteractive>(interactive);

		if (ToggleInteractive)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] Connected Interactive found %s "), *ToggleInteractive->GetData().ID.ToString());

			ToggleInteractive->Toggle();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] Connected Interactive not found "));
	}*/

}

void ASwitchInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASwitchInteractive, ConnectedInteractiveID);

}