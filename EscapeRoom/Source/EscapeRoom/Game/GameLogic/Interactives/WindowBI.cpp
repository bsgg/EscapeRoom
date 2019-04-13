// Fill out your copyright notice in the Description page of Project Settings.

#include "WindowBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
#include "Game/GameLogic/Objects/ObjectHelper.h"

#include "UnrealNetwork.h"

AWindowBI::AWindowBI()
{
	SharedObjectPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Shared Object Position"));
	SharedObjectPosition->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void AWindowBI::BeginPlay()
{
	Super::BeginPlay();
}


void AWindowBI::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

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

			CurrentController->RemoveItemFromInventory(SelectedObject);

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

			CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS, 2.0f);
			
		}
	}
}


void AWindowBI::ServerDoBorrowObject_Implementation()
{
	DoBorrowObject();
}

bool AWindowBI::ServerDoBorrowObject_Validate()
{
	return true;
}

void AWindowBI::DoBorrowObject()
{
	if (bObjectSpawned)
	{
		bObjectSpawned = false;
		ObjectSpawnedID = "None";

		PlayInteractSound();

		if (SharedObject != nullptr)
		{
			SharedObject->Destroy();
		}
	}
}

void AWindowBI::ServerDoLendObject_Implementation(const FName& ObjectID)
{
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

		FObjectInteraction* Object = UObjectHelper::GetObjectByID(ObjectID);
		if ((Object != nullptr) && (Object->ObjectMesh != nullptr))
		{
			FActorSpawnParameters SpawnParamenters;
			SpawnParamenters.Owner = this;
			SpawnParamenters.Instigator = Instigator;

			FVector Location = GetActorLocation();

			if (SharedObjectPosition != nullptr)
			{
				Location = SharedObjectPosition->GetComponentTransform().GetLocation();				
			}

			SharedObject = GetWorld()->SpawnActor<AActor>(Object->ObjectMesh, Location, FRotator::ZeroRotator, SpawnParamenters);
			if (SharedObject != nullptr)
			{
				SharedObject->SetReplicates(true);
				bObjectSpawned = true;

				PlayInteractSound();
			}
		}
	}
}


void AWindowBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWindowBI, ObjectSpawnedID);

	DOREPLIFETIME(AWindowBI, bObjectSpawned);

	DOREPLIFETIME(AWindowBI, SharedObject);
}