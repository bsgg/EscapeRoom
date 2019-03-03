// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactive.h"
#include "PickupInteractive.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API APickupInteractive : public AInteractive
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Interactive Settings", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

protected:
	virtual void BeginPlay() override;

public:
	APickupInteractive();

	FORCEINLINE FName GetPickupObjectID() const { return PickupObjectID; }

	void PickupObject();

	FString GetDetailPickup() const;	

protected:

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Pickup Interactive Settings")
	FName PickupObjectID;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Interactive Settings")
	FString DetailPickup;	

	virtual void OnRep_ObjectChanged() override;
	
};
