// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactive.h"
#include "SwitchInteractive.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API ASwitchInteractive : public AInteractive
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Interactive Settings")
	FName ConnectedInteractiveID;

public:

	void Toggle();
	
};
