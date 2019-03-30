// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "UnrealNetwork.h"


UInventoryComponent::UInventoryComponent()
{
	ObjectNum = 0;
	SetIsReplicated(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddObject(FName ObjectID, const FObjectInteraction& Object)
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
		Objects.Add(Object);

		UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::AddObject] New Object (OBJECT) Added: %s"), *Object.ID.ToString());

		if (CheckIfObjectExists(ObjectID)) return;
	  
		// Only the server is allow to change Objects
		// GetOwnerRole is the only one who has Role, not a component
	
		UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::AddObject] New Object Added: %s"), *ObjectID.ToString());
		ObjectIDs.Add(ObjectID);

		ObjectNum = ObjectIDs.Num();
	}	

}

void UInventoryComponent::RemoveObject(const FName& ObjID)
{
	if (ObjID == "NONE")
	{
		UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::RemoveObject] The Object is NONE"));
		return;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (CheckIfObjectExists(ObjID))
		{
			ObjectIDs.Remove(ObjID);
			ObjectNum = ObjectIDs.Num();

		}
	}
}


bool UInventoryComponent::CheckIfObjectExists(const FName& ObjID)
{
	FName None = FName(TEXT("NONE"));
	if (ObjID == None) return false;
	
	bool objFound = false;
	for (FName Obj : ObjectIDs)
	{
		if (Obj == ObjID)
		{
			return true;
		}
	}

	return false;
}

FName UInventoryComponent::GetObjectAt(int index) const
{
	if (index < Objects.Num())
	{
		return ObjectIDs[index];
	}

	return FName(TEXT("NONE"));
}


void UInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, ObjectIDs);

	DOREPLIFETIME(UInventoryComponent, ObjectNum);

	DOREPLIFETIME(UInventoryComponent, Objects);
}

