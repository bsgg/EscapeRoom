// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Utils/Definitions.h"

#include "RoomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API ARoomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARoomGameMode();

	virtual void BeginPlay() override;

	virtual void HandleSeamlessTravelPlayer(AController *& PlayerController) override;

	void CompletedRoom(APawn* InstigatorPawn, bool bSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "GamePlay Mode", meta = (DisplayName = "Room Completed"))
	void OnRoomCompleted(APawn* InstigatorPawn, bool bSuccess);

	FObjectInteraction* GetObjectByID(FName ID) const;

	bool FindInteractiveById(FName ID) const;


	//FInteractionData* GetInteractableByID(FName ID) const;

	//FActionData GetInteractableSecondaryAction(FName ID) const;
	

protected:

	// NOT SEAMLESS TRAVEL: Should only be called if played from the editor
	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

private:

	void GetRespawnPoints();

	void GetInteractivesInRoom();



private:
	UPROPERTY(EditAnywhere, Category = "Characters")
	TArray<FCharacterType> AvalaibleCharacters;

	// List of player after travel to Room Map
	TArray<class ALobbyPlayerController*> PlayerInGameList;
	int NumTravellers = 0;	

	UPROPERTY(Replicated)
	TArray<class ARoomPlayerStart*> SpawnPoints;

	TArray<class AInteractiveBase*> InteractiveList;


	UPROPERTY(EditDefaultsOnly)
	class UDataTable* InteractableDB; // Interactables database database

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ObjectDB; // Objects database database
	
};
