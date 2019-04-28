// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/BasicInteractive.h"
#include "ToggleBI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AToggleBI : public ABasicInteractive
{
	GENERATED_BODY()

public:
	AToggleBI();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ObjectToToggle;

	UPROPERTY(ReplicatedUsing = OnRep_IsOnChanged, EditDefaultsOnly, Category = "Interactive Settings")
	bool IsOn;

	UPROPERTY(EditDefaultsOnly, Category = "Switch Settings")
	FName ToggleOnDetail;

	UPROPERTY(EditDefaultsOnly, Category = "Switch Settings")
	FName ToggleOffDetail;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_IsOnChanged();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoToggleAction();

	void DoToggleAction();	

public:

	virtual void InteractOnConnectedInteractive() override;

	/////// IInteractiveInterface IMPLEMENTATION /////////

	virtual void StartInteract(APawn* PawnInstigator) override;

	/////// IInteractiveInterface IMPLEMENTATION /////////	
};
