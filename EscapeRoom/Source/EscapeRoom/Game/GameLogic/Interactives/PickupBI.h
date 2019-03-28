// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/BasicInteractive.h"
#include "PickupBI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API APickupBI : public ABasicInteractive
{
	GENERATED_BODY()

public:

	APickupBI();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Interactive", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(ReplicatedUsing = OnRep_PickupActionChanged, EditDefaultsOnly, Category = "Pickup Interactive Settings")
	FActionDefinition PickupAction;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_PickupActionChanged();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoInteractAction();

	void DoInteractAction();

public:

	/////// IInteractiveInterface IMPLEMENTATION /////////

	virtual void StartInteract(APawn* Instigator) override;

	/////// IInteractiveInterface IMPLEMENTATION /////////	
};
