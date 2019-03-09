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
	
	void AddObject(FName ObjectID, const FObjectInteraction& Object);

	bool CheckIfObjectExists(FName ObjectID);

	void RemoveObject(FName ObjectID);
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	int ObjectNum;

	FORCEINLINE int GetNumObjects() const { return Objects.Num(); }

	FORCEINLINE TArray<FObjectInteraction> GetObjects() const { return Objects; }

	FName GetObjectAt(int index) const;	

protected:

	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	TArray<FName> ObjectIDs;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	TArray<FObjectInteraction> Objects;
};
