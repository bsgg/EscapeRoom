// Fill out your copyright notice in the Description page of Project Settings.

#include "UseInteractive.h"
#include "UnrealNetwork.h"


void AUseInteractive::Use()
{
	if (Role == ROLE_Authority)
	{
		OnUseEvent();
	}
}


void AUseInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUseInteractive, UseObjectID);

	DOREPLIFETIME(AUseInteractive, bNeedsObjectToUse);
}

