// Fill out your copyright notice in the Description page of Project Settings.

#include "UseBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"

AUseBI::AUseBI()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void AUseBI::StartInteract(APawn* Instigator)
{
	Super::StartInteract(Instigator);

	if (CurrentController == nullptr) return;

	if (UseAction.IsActive)
	{
		if (UseAction.HasObject())
		{
			// Check if Player Controller has the object selected
			if (CurrentController->GetSelectedItem() == UseAction.ObjectID)
			{
				CurrentController->ShowMessage(UseAction.DetailDefaultAction.ToString());

				CurrentController->RemoveItemFromInventory(UseAction.ObjectID);

				if (CharacterOverlapping != nullptr)
				{
					CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT);
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
			else
			{
				CurrentController->ShowMessage(UseAction.DetailWrongAction.ToString());

				if (CharacterOverlapping != nullptr)
				{
					CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS);
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("[APickupBI::StartInteract] %s "), *CurrentController->GetSelectedItem().ToString());



			// Check if Player Controller has the object selected
			// Remove the object from Player Inventory

			// Finish action use

			// Show message in player

			

		}
		else
		{
			CurrentController->ShowMessage(UseAction.DetailDefaultAction.ToString());			

			if (CharacterOverlapping != nullptr)
			{
				CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS);
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
	OnUseEvent();

	UseAction.IsActive = false;

	OnRep_UseActionChanged();

	if (UseAction.HasAnimation())
	{
		// Set timer
		GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &AUseBI::OnEndAnimationTimer, UseAction.AnimationLength, false);
	}
	else
	{
		// Enable Pickup action
		if (PickupAction.HasObject() && (!PickupAction.IsActive))
		{
			PickupAction.IsActive = true;
			OnRep_PickupActionChanged();
		}
	}
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
