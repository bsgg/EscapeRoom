// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	ObjectNum = 0;
	SetIsReplicated(true);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	

	// ...
	
}

void UInventoryComponent::AddObject(FName ObjectID)
{
	// Check if this object already exists
	FName None = FName(TEXT("NONE"));
	if (ObjectID == None)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::AddObject] The Object is NONE"));
		return;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (CheckIfObjectExists(ObjectID)) return;
	  
		// Only the server is allow to change Objects
		// GetOwnerRole is the only one who has Role, not a component
	
		UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::AddObject] New Object Added: %s"), *ObjectID.ToString());
		Objects.Add(ObjectID);

		ObjectNum = Objects.Num();	
	}	

	// Broadcast this change 
	//OnInventoryChanged.Broadcast(this, ObjectID, Objects.Num());
}

void UInventoryComponent::RemoveObject(FName ObjectID)
{
	if (ObjectID == "NONE")
	{
		UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::RemoveObject] The Object is NONE"));
		return;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (CheckIfObjectExists(ObjectID))
		{
			Objects.Remove(ObjectID);
			ObjectNum = Objects.Num();

		}
	}
}


bool UInventoryComponent::CheckIfObjectExists(FName ObjectID)
{
	FName None = FName(TEXT("NONE"));
	if (ObjectID == None) return false;
	
	bool objFound = false;
	for (FName Obj : Objects)
	{
		if (Obj == ObjectID)
		{
			return true;
		}
	}

	return false;
}


void UInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Objects);

	DOREPLIFETIME(UInventoryComponent, ObjectNum);
}

