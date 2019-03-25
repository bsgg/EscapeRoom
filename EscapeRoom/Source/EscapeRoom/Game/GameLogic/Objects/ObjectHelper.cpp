// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectHelper.h"

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


FObjectInteraction* UObjectHelper::GetObjectByID(FName ID)
{
	FObjectInteraction* Row = ObjectsTable->FindRow<FObjectInteraction>(ID, ContextString);

	return Row;
}