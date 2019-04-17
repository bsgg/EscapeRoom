// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Utils/Definitions.h"
#include "ObjectHelper.generated.h"


USTRUCT(BlueprintType)
struct FInteractiveUI : public FTableRowBase
{
	GENERATED_BODY()

public:
	FInteractiveUI()
	{		
		ID = "Obj_";
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ExtraInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> UI;

	bool operator==(const FInteractiveUI& object) const
	{
		return (ID == object.ID);
	}
};


static UDataTable* ObjectsTable;

static UDataTable* WidgetDataTable;

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

	static FObjectInteraction* GetObjectByID(const FName& ID);

	static UStaticMesh* GetObjectMesh(const FName& ID);


	static FInteractiveUI* GetWidgetByID(const FName& ID);
	
};
