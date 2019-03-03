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

	UE_LOG(LogTemp, Warning, TEXT("[APickupInteractive::BeginPlay]"));

	if (IsActive)
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
	PickupObjectID = "None";

	IsActive = false;
	
	OnRep_ObjectChanged();

	Definition.IsDefaultDetailInspectActive = true;

	UE_LOG(LogTemp, Warning, TEXT("[APickupInteractive::PickupObject]"));
}

FString APickupInteractive::GetDetailPickup() const
{
	return DetailPickup;
}

void APickupInteractive::OnRep_ObjectChanged()
{
	Super::OnRep_ObjectChanged();
	
	if (IsActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("[APickupInteractive::OnRep_ObjectChanged] IS ACTIVE"));

		AInputIconMesh->SetVisibility(true);
		PickupMesh->SetVisibility(true, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[APickupInteractive::OnRep_ObjectChanged] IS NO ACTIVE"));
		AInputIconMesh->SetVisibility(false);
		PickupMesh->SetVisibility(false, true);
	}
}

void APickupInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupInteractive, PickupObjectID);
}