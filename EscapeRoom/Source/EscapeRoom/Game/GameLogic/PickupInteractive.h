// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactive.h"
#include "Utils/Definitions.h"
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

	FORCEINLINE FActionDefinition GetPickupAction() const { return PickupAction; }

	void PickupObject();

protected:
	
	virtual void OnRep_DefinitionChanged(FInteractiveDefinition PreviousData) override;

	UPROPERTY(ReplicatedUsing = OnRep_PickupActionChanged, EditDefaultsOnly, Category = "Use Interactive Settings")
	FActionDefinition PickupAction;

	UFUNCTION()
	void OnRep_PickupActionChanged();	
};
