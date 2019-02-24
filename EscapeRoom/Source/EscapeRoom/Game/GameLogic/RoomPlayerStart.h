// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Utils/Definitions.h"
#include "RoomPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API ARoomPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	FORCEINLINE ECharacterType GetCharacterToSpawn() const { return CharacterToSpawn; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	ECharacterType CharacterToSpawn;
	
};
