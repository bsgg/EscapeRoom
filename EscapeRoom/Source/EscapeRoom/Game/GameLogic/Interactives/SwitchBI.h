// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/BasicInteractive.h"
#include "SwitchBI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API ASwitchBI : public ABasicInteractive
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Switch Settings")
	FName ConnectedInteractiveID;	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Switch Settings")
	bool IsSwitchOn = true;

	UPROPERTY(EditDefaultsOnly, Category = "Switch Settings")
	USoundBase* OverlappedSound;

	UPROPERTY(EditDefaultsOnly, Category = "Switch Settings")
	FName SwitchOnDetail;

	UPROPERTY(EditDefaultsOnly, Category = "Switch Settings")
	FName SwitchOffDetail;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoToggleAction();

	void DoToggleAction();

public:

	/////// IInteractiveInterface IMPLEMENTATION /////////

	virtual void StartInteract(APawn* Instigator) override;

	/////// IInteractiveInterface IMPLEMENTATION /////////	
	
};
