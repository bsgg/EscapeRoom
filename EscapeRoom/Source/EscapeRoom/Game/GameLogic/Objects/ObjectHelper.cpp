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

	static ConstructorHelpers::FObjectFinder<UDataTable> WidgetAsset(TEXT("/Game/Game/Data/WidgetDataTable"));

	if (WidgetAsset.Object)
	{
		WidgetDataTable = WidgetAsset.Object;
	}
}


FObjectInteraction* UObjectHelper::GetObjectByID(const FName& ID)
{
	FObjectInteraction* Row = ObjectsTable->FindRow<FObjectInteraction>(ID, ContextString);

	return Row;
}

FInteractiveUI* UObjectHelper::GetWidgetByID(const FName& ID)
{
	FInteractiveUI* Row = WidgetDataTable->FindRow<FInteractiveUI>(ID, ContextString);

	return Row;
}