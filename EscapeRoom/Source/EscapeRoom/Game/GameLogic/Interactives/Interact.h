// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 * The job of this interface is simply to receive messages when the implementer is in and out of range of interactables and they can handle it as they wish.
 */
class ESCAPEROOM_API IInteract
{
	GENERATED_BODY()

public:

	virtual void NotifyInInteractRange(AActor* Interactive) = 0;

	virtual void NotifyLeaveInteractRange(AActor* Interactive) = 0;

};
