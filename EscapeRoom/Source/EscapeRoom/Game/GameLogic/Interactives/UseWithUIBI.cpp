// Fill out your copyright notice in the Description page of Project Settings.


#include "UseWithUIBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"


AUseWithUIBI::AUseWithUIBI()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void AUseWithUIBI::BeginPlay()
{
	Super::BeginPlay();

}

void AUseWithUIBI::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

	if (CurrentController == nullptr) return;

	if (IsCompleted)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::StartInteract] Completed"));

		CurrentController->ShowMessage(CompletedActionMessage, 2.0f);

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS, 2.0f);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::StartInteract] Incompleted"));

		ShowWidget();

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
		}
	}
}

void AUseWithUIBI::OnComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::OnComplete] Called"));

	Super::OnComplete();

	// Check if it has pickup
	if (PickupAction.IsActive && PickupAction.HasObject())
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::OnComplete] PICKUP "));

		if (CurrentController == nullptr) return;

		if ((PickupAction.HasObject()) && (PickupAction.IsActive))
		{
			CurrentController->ShowMessage(PickupAction.DetailDefaultAction.ToString(), 2.0f);

			CurrentController->AddItemToInventory(PickupAction.ObjectID);

			if (CharacterOverlapping != nullptr)
			{
				CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
			}
		}
		else
		{
			CurrentController->ShowMessage(PickupAction.DetailWrongAction.ToString(), 2.0f);

			if (CharacterOverlapping != nullptr)
			{
				CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS, 2.0f);
			}
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
	else // No Pickup
	{
		if (Role < ROLE_Authority)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::OnComplete]  NO PICKUP. ServerDoAction"));
			ServerDoAction();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::OnComplete] NO PICKUP. Do Action"));
			DoAction();
		}
	}
}



void AUseWithUIBI::ServerDoAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::ServerDoAction_Implementation]"));

	DoAction();
}

bool AUseWithUIBI::ServerDoAction_Validate()
{
	return true;
}

void AUseWithUIBI::DoAction()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction]"));

	if (bDoActionOnThisInteractive)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction] Acting over this interactive "));

		OnCompleteActionEvent();
	}

	return;

	ABasicInteractive* interactive = FindConnectedInteractive();

	if (interactive != nullptr)
	{
		IsCompleted = true;

		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction] Connected Interactive found "));

		interactive->InteractOnConnectedInteractive();		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction] Connected Interactive not found "));
	}

}


void AUseWithUIBI::ServerDoPickupAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::ServerDoPickupAction]"));

	DoPickupAction();
}
bool AUseWithUIBI::ServerDoPickupAction_Validate()
{
	return true;
}

void AUseWithUIBI::DoPickupAction()
{
	// Set this object as a completed
	IsCompleted = true;

	if ((PickupAction.HasObject()) && (PickupAction.IsActive))
	{
		PickupAction.ObjectID = "None";

		PickupAction.IsActive = false;

		Properties.EnableDefaultInspectMessage = true;

		PickupMesh->SetVisibility(false, true);

		PlayInteractSound();
	}
}


void AUseWithUIBI::OnRep_PickupActionChanged()
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


void AUseWithUIBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUseWithUIBI, IsCompleted);

	DOREPLIFETIME(AUseWithUIBI, PickupAction);

	
}

