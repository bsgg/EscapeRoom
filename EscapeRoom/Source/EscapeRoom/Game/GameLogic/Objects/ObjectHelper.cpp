// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectHelper.h"
#include "Engine/StreamableManager.h"

UObjectHelper::UObjectHelper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	static ConstructorHelpers::FObjectFinder<UDataTable> TableAsset(TEXT("/Game/Game/Data/Objects.Objects"));

	if (TableAsset.Object)
	{

		ObjectsTable = TableAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> UITableAsset(TEXT("/Game/Game/Data/InteractiveUITable"));

	if (UITableAsset.Object)
	{
		InteractiveUITable = TableAsset.Object;
	}
}


FObjectInteraction* UObjectHelper::GetObjectByID(const FName& ID)
{
	FObjectInteraction* Row = ObjectsTable->FindRow<FObjectInteraction>(ID, ContextString);

	return Row;
}

UStaticMesh* UObjectHelper::GetObjectMesh(const FName& ID)
{
	FObjectInteraction* Row = ObjectsTable->FindRow<FObjectInteraction>(ID, ContextString);

	if (Row != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UObjectHelper::GetObjectMesh]Row not  null"));

		return (Cast<UStaticMesh>(Row->Mesh.Get()));

		//Row->Mesh.ToStringReference()

		/*if (Row->Mesh.IsPending())
		{
			Row->Mesh.LoadSynchronous()

			const FSoftObjectPath& AssetRef = Row->Mesh.ToStringReference();
			Row->Mesh = Cast<UStaticMesh>(Streamable.SynchronousLoad(AssetRef));
		}
		return Row->Mesh.Get();*/
	}


	return nullptr;
}


FInteractiveUI* UObjectHelper::GetInteractiveUIByID(const FName& ID)
{
	FInteractiveUI* Row = InteractiveUITable->FindRow<FInteractiveUI>(ID, ContextString);

	return Row;
}