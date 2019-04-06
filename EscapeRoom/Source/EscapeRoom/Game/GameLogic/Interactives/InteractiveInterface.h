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

public:

	virtual void Inspect(APawn* Instigator) = 0;

	virtual void StartInteract(APawn* Instigator) = 0;

	virtual FString GetInteractID() = 0;
};
