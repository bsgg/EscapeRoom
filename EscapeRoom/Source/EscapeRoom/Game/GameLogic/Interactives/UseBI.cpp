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
				CurrentController->ShowMessage(UseAction.DetailDefaultAction.ToString(), 2.0f);

				CurrentController->RemoveItemFromInventory(UseAction.ObjectID);

				if (CharacterOverlapping != nullptr)
				{
					CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
				}

				if (Role < ROLE_Authority)
				{
					ServerDoUseAction();
				}
				else
				{
					DoUseAction();
				}
			}
			else
			{
				CurrentController->ShowMessage(UseAction.DetailWrongAction.ToString(), 2.0f);

				if (CharacterOverlapping != nullptr)
				{
					CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS, 2.0f);
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("[APickupBI::StartInteract] %s "), *CurrentController->GetSelectedItem().ToString());
			
		}
		

		if (!UseAction.HasObject())
		{
			CurrentController->ShowMessage(UseAction.DetailDefaultAction.ToString(), 2.0f);

			if (CharacterOverlapping != nullptr)
			{
				CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS,2.0f);
			}

			if (Role < ROLE_Authority)
			{
				ServerDoUseAction();
			}
			else
			{
				DoUseAction();
			}

		}
	}

	if (PickupAction.IsActive && PickupAction.HasObject())
	{
		CurrentController->ShowMessage(PickupAction.DetailDefaultAction.ToString(), 2.0f);

		CurrentController->AddItemToInventory(PickupAction.ObjectID);

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
		}

		if (Role < ROLE_Authority)
		{
			ServerDoPickupAction();
		}
		else
		{
			DoPickupAction();
		}
	}

}


void AUseBI::ServerDoUseAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseBI::ServerDoInteractAction]"));

	DoUseAction();
}

bool AUseBI::ServerDoUseAction_Validate()
{
	return true;
}

void AUseBI::DoUseAction()
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

void AUseBI::ServerDoPickupAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseBI::ServerDoPickupAction]"));

	DoPickupAction();
}
bool AUseBI::ServerDoPickupAction_Validate()
{
	return true;
}

		
void AUseBI::DoPickupAction()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseBI::DoPickupAction]"));

	if ((PickupAction.HasObject()) && (PickupAction.IsActive))
	{
		PickupAction.ObjectID = "None";

		PickupAction.IsActive = false;

		Properties.EnableDefaultInspectMessage = true;

		PickupMesh->SetVisibility(false, true);
	}
}


void AUseBI::OnRep_PickupActionChanged()
{
	if (PickupAction.HasObject())
	{
		PickupMesh->SetVisibility(true, true);
	}
	else
	{
		PickupMesh->SetVisibility(false, true);
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
