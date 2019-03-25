// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Utils/Definitions.h"
#include "ObjectHelper.generated.h"


static UDataTable* ObjectsTable;

static const FString ContextString(TEXT("GENERAL"));

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UObjectHelper : public UObject
{
	GENERATED_BODY()

public:
	UObjectHelper(const FObjectInitializer& ObjectInitializer);

	static FObjectInteraction* GetObjectByID(FName ID);
	
};
