// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
#include "UnrealNetwork.h"

APickupBI::APickupBI()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void APickupBI::BeginPlay()
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

void APickupBI::StartInteract(APawn* Instigator)
{
	Super::StartInteract(Instigator);

	if (CurrentController == nullptr) return;

	if ((PickupAction.HasObject()) && (PickupAction.IsActive))
	{
		CurrentController->ShowMessage(PickupAction.DetailDefaultAction.ToString());

		CurrentController->AddItemToInventory(PickupAction.ObjectID);

		// TODO: PICKUP OBJECT AND ADDED IT IN THE INVENTORY

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT);
		}
	}
	else
	{
		CurrentController->ShowMessage(PickupAction.DetailWrongAction.ToString());

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS);
		}
	}

	if (Role < ROLE_Authority)
	{
		ServerDoInteractAction();
	}
	else
	{
		DoInteractAction();
	}
}

void APickupBI::ServerDoInteractAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[ABasicInteractive::ServerDoInteractAction]"));

	DoInteractAction();
}

bool APickupBI::ServerDoInteractAction_Validate()
{
	return true;
}

void APickupBI::DoInteractAction()
{
	if ((PickupAction.HasObject()) && (PickupAction.IsActive))
	{
		PickupAction.ObjectID = "None";

		PickupAction.IsActive = false;

		Properties.EnableDefaultInspectMessage = true;

		PickupMesh->SetVisibility(false, true);
	}
}

void APickupBI::OnRep_PickupActionChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("[APickupBI::OnRep_PickupActionChanged] Changed"));

	if (PickupAction.IsActive)
	{
		PickupMesh->SetVisibility(true, true);
	}
	else
	{
		PickupMesh->SetVisibility(false, true);
	}
}

void APickupBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupBI, PickupAction);
}
