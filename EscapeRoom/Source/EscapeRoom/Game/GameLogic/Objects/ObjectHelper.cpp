// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectHelper.h"
#include "Engine/StreamableManager.h"

UObjectHelper::UObjectHelper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	UE_LOG(LogTemp, Warning, TEXT("[UObjectHelper] Called "));

	static ConstructorHelpers::FObjectFinder<UDataTable> TableAsset(TEXT("/Game/Game/Data/Objects.Objects"));

	if (TableAsset.Object)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UObjectHelper] TableAssetExist"));

		ObjectsTable = TableAsset.Object;
	}
}

//Get method to access the instance
/*UObjectHelper& UObjectHelper::Get()
{
	UObjectHelper *Singleton = Cast<UObjectHelper>(GEngine->GameSingleton);

	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogFort, Fatal, TEXT("Invalid singleton"));
		return *ConstructObject<UObjectHelper>(UObjectHelper::StaticClass()); // never calls this
	}
}*/

/*
// https://docs.unrealengine.com/en-US/Programming/Assets/AsyncLoading
void UObjectHelper::LoadMeshes()
{
	TArray<FSoftObjectPath> ItemsToStream;
	FStreamableManager& Streamable = UGameGlobals::Get().StreamableManager;

	for (auto it : ObjectsTable->GetRowMap())
	{
		// it.Key has the key from first column of the CSV file
		// it.Value has a pointer to a struct of data. You can safely cast it to your actual type, e.g FMyStruct* data = (FMyStruct*)(it.Value);

		FObjectInteraction* data = (FObjectInteraction*)(it.Value);
		ItemsToStream.AddUnique(data->Mesh.ToStringReference());
	}

	Streamable.RequestAsyncLoad(ItemsToStream, FStreamableDelegate::CreateUObject(this, &UObjectHelper::LoadMeshesDeferred));
}

void UObjectHelper::LoadMeshesDeferred()
{
	for (auto it : ObjectsTable->GetRowMap())
	{
		UStaticMesh* MeshData = it.Value->Mesh.Get();
	}
}
*/

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