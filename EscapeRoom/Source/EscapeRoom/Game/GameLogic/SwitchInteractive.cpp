// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchInteractive.h"
#include "Game/RoomGameMode.h"
#include "UnrealNetwork.h"


void ASwitchInteractive::Toggle()
{
	UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] %s "), *ConnectedInteractiveID.ToString());

	ARoomGameMode* GM = Cast<ARoomGameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr) return;

	if (GM->FindInteractiveById(ConnectedInteractiveID))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] Connected Interactive found "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ASwitchInteractive::Toggle] Connected Interactive not found "));
	}

}


void ASwitchInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASwitchInteractive, ConnectedInteractiveID);

}