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

	AInputIconMesh->SetVisibility(false);
}


void APickupInteractive::PickupObject()
{
	UE_LOG(LogTemp, Warning, TEXT("[ APickupInteractive::PickupObject] Called "));

	ObjectID = "None";

	IsActive = false;
	
	OnRep_PickupChanged();

	Definition.IsDefaultDetailInspectActive = true;
}

FString APickupInteractive::GetDetailPickup() const
{
	return DetailPickup;
}

void APickupInteractive::OnRep_PickupChanged()
{
	if (!IsActive)
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



void APickupInteractive::OnRep_DefinitionChanged(FInteractiveDefinition PreviousData)
{

	Super::OnRep_DefinitionChanged(PreviousData);

	if (Definition.IsLocked)
	{
		if (IsActive)
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



void APickupInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupInteractive, ObjectID);

	DOREPLIFETIME(APickupInteractive, IsActive);
}
