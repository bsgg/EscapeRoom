// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupInteractive.h"

#include "UnrealNetwork.h"

APickupInteractive::APickupInteractive()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void APickupInteractive::BeginPlay()
{
	Super::BeginPlay();	

	if (PickupAction.IsActive)
	{		
		PickupMesh->SetVisibility(true, true);
	}
	else
	{		
		PickupMesh->SetVisibility(false, true);
	}
}


void APickupInteractive::PickupObject()
{
	PickupAction.ObjectID = "None";

	PickupAction.IsActive = false;
	
	OnRep_PickupActionChanged();

	Definition.IsDefaultDetailInspectActive = true;	
}

void APickupInteractive::OnRep_DefinitionChanged(FInteractiveDefinition PreviousData)
{
	Super::OnRep_DefinitionChanged(PreviousData);

	if (PickupAction.IsActive)
	{
		if (Definition.IsLocked)
		{
			AInputIconMesh->SetVisibility(true);
		}
		else
		{
			AInputIconMesh->SetVisibility(false);
		}		
	}
	else
	{
		AInputIconMesh->SetVisibility(false);
	}

}


void APickupInteractive::OnRep_PickupActionChanged()
{	
	if (PickupAction.IsActive)
	{
		AInputIconMesh->SetVisibility(true);
		PickupMesh->SetVisibility(true, true);
	}
	else
	{
		AInputIconMesh->SetVisibility(false);
		PickupMesh->SetVisibility(false, true);

		DisableCollider();
	}
}

void APickupInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupInteractive, PickupAction);
}