// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utils/Definitions.h"

#include "InventoryComponent.generated.h"

// OnInventoryChanged event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryChangedSignature, UInventoryComponent*, InventoryComp, FName, ObjectID, int32, NumberObjects);

UCLASS( ClassGroup=(INVENTORY), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInventoryChangedSignature OnInventoryChanged;
	
	void AddObject(FName ObjectID);

	bool CheckIfObjectExists(FName ObjectID);
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	int ObjectNum;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	TArray<FName> Objects;
		
};
