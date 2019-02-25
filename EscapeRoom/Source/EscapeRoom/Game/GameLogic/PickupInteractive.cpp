// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupInteractive.h"

#include "UnrealNetwork.h"

APickupInteractive::APickupInteractive()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);
}


void APickupInteractive::PickupObject()
{
	UE_LOG(LogTemp, Warning, TEXT("[ APickupInteractive::PickupObject] Called "));

	PickupMesh->SetVisibility(false);

	ObjectID = "None";

	IsActive = false;

	AInputIconMesh->SetVisibility(true);
}


void APickupInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupInteractive, ObjectID);

	DOREPLIFETIME(APickupInteractive, IsActive);
}