// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/BasicInteractive.h"
#include "BasicInspectBI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API ABasicInspectBI : public ABasicInteractive
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Basic Inspect Settings")
	FString InspectMessage;

	FTimerHandle WidgetTimerHandle;
	void OnShowWidget();

public:

	/////// IInteractiveInterface IMPLEMENTATION /////////

	virtual void StartInteract(APawn* PawnInstigator) override;

	/////// IInteractiveInterface IMPLEMENTATION /////////	
	
};
