// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EscapeRoomGameState.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AEscapeRoomGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnRoomCompleted(APawn* InstigatorPawn, bool bSuccess);
	
};
