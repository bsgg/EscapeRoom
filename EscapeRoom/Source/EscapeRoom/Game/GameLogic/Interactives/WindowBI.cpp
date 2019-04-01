// Fill out your copyright notice in the Description page of Project Settings.

#include "WindowBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
#include "UnrealNetwork.h"

AWindowBI::AWindowBI()
{
	ObjectSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("Object Spawner"));
	ObjectSpawner->SetupAttachment(RootComponent);

	SetReplicates(true);
}


void AWindowBI::StartInteract(APawn* Instigator)
{
	Super::StartInteract(Instigator);

	if ((CurrentController == nullptr) || (CharacterOverlapping == nullptr)) return;

	if (bObjectSpawned)
	{
		// Pickup the object, destroy the current object, set to none
		CurrentController->ShowMessage(BorrowObjectMessage.ToString(), 2.0f);

		CurrentController->AddItemToInventory(ObjectSpawnedID);

		CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);		

		if (Role < ROLE_Authority)
		{
			ServerDoBorrowObject();
		}
		else
		{
			DoBorrowObject();
		}

	}
	else if (!bObjectSpawned)
	{
		// No object, check if controller has an item selected
		FName SelectedObject = CurrentController->GetSelectedItem();
		if (SelectedObject != "NONE")
		{
			CurrentController->ShowMessage(LendObjectMessage.ToString(), 2.0f);

			CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);

			if (Role < ROLE_Authority)
			{
				ServerDoLendObject(SelectedObject);
			}
			else
			{
				DoLendObject(SelectedObject);
			}
		}
		else
		{
			CurrentController->ShowMessage(NoObjectMessage.ToString(), 2.0f);

			CurrentController->RemoveItemFromInventory(SelectedObject);

			CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS, 2.0f);
			
		}
	}

}


void AWindowBI::ServerDoBorrowObject_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AWindowBI::ServerDoBorrowObject]"));

	DoBorrowObject();
}

bool AWindowBI::ServerDoBorrowObject_Validate()
{
	return true;
}

void AWindowBI::DoBorrowObject()
{
	UE_LOG(LogTemp, Warning, TEXT("[AWindowBI::DoBorrowObject]"));
	if (bObjectSpawned)
	{
		bObjectSpawned = false;
		ObjectSpawnedID = "None";

		if (SpawnedObject != nullptr)
		{
			//Destroy(SpawnedObject);
		}
	}
}




void AWindowBI::ServerDoLendObject_Implementation(const FName& ObjectID)
{
	UE_LOG(LogTemp, Warning, TEXT("[AWindowBI::ServerDoLendObject]"));

	DoLendObject(ObjectID);
}

bool AWindowBI::ServerDoLendObject_Validate(const FName& ObjectID)
{
	return true;
}

void AWindowBI::DoLendObject(const FName& ObjectID)
{
	if (!bObjectSpawned)
	{
		// Find the object
		ObjectSpawnedID = ObjectID;

		// Find the object and spawned

		bObjectSpawned = true;
	}
}


void AWindowBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWindowBI, ObjectSpawnedID);

	DOREPLIFETIME(AWindowBI, bObjectSpawned);

	DOREPLIFETIME(AWindowBI, SpawnedObject);
}