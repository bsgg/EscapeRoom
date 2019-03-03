// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactive.h"
#include "UseInteractive.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AUseInteractive : public AInteractive
{
	GENERATED_BODY()

public:

	FORCEINLINE FName GetUseObjectID() const { return UseObjectID; }

	FORCEINLINE bool NeedsObjectToUse() const { return bNeedsObjectToUse;}

	void Use();	

	UFUNCTION(BlueprintImplementableEvent)
	void OnUseEvent();

protected:

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Use Interactive Settings")
	FName UseObjectID;	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Use Interactive Settings")
	bool bNeedsObjectToUse;

	
};
