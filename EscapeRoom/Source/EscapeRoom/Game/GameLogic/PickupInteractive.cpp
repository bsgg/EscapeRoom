// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupInteractive.h"

#include "UnrealNetwork.h"

APickupInteractive::APickupInteractive()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);
}

void APickupInteractive::BeginPlay()
{
	Super::BeginPlay();	

	if (bHasObjectToPickup)
	{
		AInputIconMesh->SetVisibility(true);

		PickupMesh->SetVisibility(true);
	}
}


void APickupInteractive::PickupObject()
{
	ObjectID = "None";

	IsActive = false;

	bHasObjectToPickup = false;
	
	OnRep_ObjectChanged();

	Definition.IsDefaultDetailInspectActive = true;
}

FString APickupInteractive::GetDetailPickup() const
{
	return DetailPickup;
}

void APickupInteractive::OnRep_ObjectChanged()
{
	if (!bHasObjectToPickup)
	{
		AInputIconMesh->SetVisibility(false);

		PickupMesh->SetVisibility(false);
	}
	else
	{
		AInputIconMesh->SetVisibility(true);

		PickupMesh->SetVisibility(true);
	}
}

void APickupInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupInteractive, bHasObjectToPickup);
}