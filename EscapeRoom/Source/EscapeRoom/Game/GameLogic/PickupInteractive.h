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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

protected:
	virtual void BeginPlay() override;

public:
	APickupInteractive();

	void PickupObject();

	FString GetDetailPickup() const;

	

protected:

	//UPROPERTY(ReplicatedUsing = OnRep_PickupChanged, EditDefaultsOnly, Category = "Interactive Settings")
	//FName ObjectID;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Settings")
	FString DetailPickup;	

	virtual void OnRep_ObjectChanged() override;
	
};
