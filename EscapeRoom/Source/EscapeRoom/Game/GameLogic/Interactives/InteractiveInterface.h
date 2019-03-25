// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ESCAPEROOM_API IInteractiveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FString GetInteractMessage() = 0;

	// Start execution of the interactive, Add items to inventory, bring up UI
	virtual void StartInteract(APawn* Instigator) = 0;

	// Finish execution of the interactive, closes UI, destroy objects, etc
	virtual void FinishInteract(APawn* Instigator) = 0;
};
