// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/BasicInteractive.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractiveInterface.h"
#include "UseWithUIBI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AUseWithUIBI : public ABasicInteractive, public IUIBasicInteractiveInterface
{
	GENERATED_BODY()

public:
	AUseWithUIBI();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Use With UI Interactive Settings")
	FName ObjectID = "NONE";

public:

	/////// IInteractiveInterface IMPLEMENTATION /////////

	virtual void StartInteract(APawn* Instigator) override;

	/////// IInteractiveInterface IMPLEMENTATION /////////	

	/////// IUIBasicInteractiveInterface IMPLEMENTATION /////////
	virtual void ExitUI() override;

	/////// IUIBasicInteractiveInterface IMPLEMENTATION /////////
	
};
