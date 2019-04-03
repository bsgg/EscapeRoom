// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Utils/Definitions.h"
#include "ObjectHelper.generated.h"


static UDataTable* ObjectsTable;

static const FString ContextString(TEXT("Objects"));

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UObjectHelper : public UObject
{
	GENERATED_BODY()

public:
	UObjectHelper(const FObjectInitializer& ObjectInitializer);

	//static UObjectHelper& Get(); 	// Get method to access this object

	//FStreamableManager AssetLoader;		// Your asset loader

	static FObjectInteraction* GetObjectByID(const FName& ID);

	static UStaticMesh* GetObjectMesh(const FName& ID);

	//void LoadMeshes();


	//void LoadMeshesDeferred();
	
	
};
