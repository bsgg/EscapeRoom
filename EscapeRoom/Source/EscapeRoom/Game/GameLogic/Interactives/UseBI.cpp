// Fill out your copyright notice in the Description page of Project Settings.

#include "UseBI.h"

AUseBI::AUseBI()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	SetReplicates(true);
}



void AUseBI::StartInteract(APawn* Instigator)
{
	Super::StartInteract(Instigator);

	if (UseAction.IsActive)
	{
		if (UseAction.HasObject())
		{
			// Check if Player Controller has the object selected
			// Remove the object from Player Inventory

			// Finish action use

			// Show message in player

			if (Role < ROLE_Authority)
			{
				ServerDoInteractAction();
			}
			else
			{
				DoInteractAction();
			}

		}
	}

}


void AUseBI::ServerDoInteractAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseBI::ServerDoInteractAction]"));

	DoInteractAction();
}

bool AUseBI::ServerDoInteractAction_Validate()
{
	return true;
}

void AUseBI::DoInteractAction()
{
	
}

void AUseBI::OnEndAnimationTimer()
{
	if (PickupAction.HasObject() && (!PickupAction.IsActive))
	{
		PickupAction.IsActive = true;
		OnRep_PickupActionChanged();
	}	
}


void AUseBI::OnRep_PickupActionChanged()
{
	if (PickupAction.HasObject())
	{
		PickupMesh->SetVisibility(true);
	}
	else
	{
		PickupMesh->SetVisibility(false);
	}
}


void AUseBI::OnRep_UseActionChanged()
{

}


void AUseBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUseBI, UseAction);

	DOREPLIFETIME(AUseBI, PickupAction);
}
