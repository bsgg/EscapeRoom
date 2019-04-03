// Fill out your copyright notice in the Description page of Project Settings.

#include "WindowBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
//#include "Game/GameLogic/Objects/ObjectHelper.h"
//#include "Engine/StreamableManager.h"
#include "UnrealNetwork.h"

AWindowBI::AWindowBI()
{
	ObjectToShare = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object To Share")); 
	ObjectToShare->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void AWindowBI::BeginPlay()
{
	Super::BeginPlay();

	// Load objects

	/*TArray<FSoftObjectPath> ItemsToStream;
	FStreamableManager& Streamable = UGameGlobals::Get().StreamableManager;

	UE_LOG(LogTemp, Warning, TEXT("[AWindowBI::BeginPlay] "));

	for (auto it : ObjectsTable->GetRowMap())
	{
		// it.Key has the key from first column of the CSV file
		// it.Value has a pointer to a struct of data. You can safely cast it to your actual type, e.g FMyStruct* data = (FMyStruct*)(it.Value);

		FObjectInteraction* data = (FObjectInteraction*)(it.Value);
		if ((data != nullptr) && (data->Mesh != nullptr))
		{
			UE_LOG(LogTemp, Warning, TEXT("[AWindowBI::BeginPlay] data not null "));

			ItemsToStream.AddUnique(data->Mesh.ToStringReference());
		}
	}

	Streamable.RequestAsyncLoad(ItemsToStream, FStreamableDelegate::CreateUObject(this, &AWindowBI::LoadMeshesDeferred));
	*/

}

/*void AWindowBI::LoadMeshesDeferred()
{
	UE_LOG(LogTemp, Warning, TEXT("[AWindowBI::LoadMeshesDeferred] "));

	for (auto it : ObjectsTable->GetRowMap())
	{
		UStaticMesh* MeshData = it.Value->Mesh.Get();

		if (MeshData != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AWindowBI::LoadMeshesDeferred] MeshData not null"));
		}
	}
}*/


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
		UStaticMesh* Mesh = UObjectHelper::GetObjectMesh(ObjectID);

		if (Mesh != nullptr)
		{

			UE_LOG(LogTemp, Warning, TEXT("[ AWindowBI::DoLendObject] Not null obj"));
			ObjectToShare->SetStaticMesh(Mesh);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[ AWindowBI::DoLendObject] null obj"));
		}

		//FObjectInteraction* ObjectToLend = UObjectHelper::GetObjectByID(ObjectID);

		//if ((ObjectToLend != nullptr) && (ObjectToLend->Mesh != nullptr))
		//{		
			//const FSoftObjectPath& AssetRef = ObjectToLend->Mesh.ToStringReference();

			//if (ObjectToLend->Mesh.IsPending())
			//{
				//const FSoftObjectPath& AssetRef = ObjectToLend->Mesh.ToStringReference();
				//BaseMesh = Cast< UStaticMesh>(Streamable.SynchronousLoad(AssetRef));
			//}
			//UStaticMesh* StaticMesh = ObjectToLend->Mesh.Get();
			//ObjectToShare->SetStaticMesh(StaticMesh);		
		//}

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