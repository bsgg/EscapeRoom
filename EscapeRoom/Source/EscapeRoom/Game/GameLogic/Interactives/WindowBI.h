// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/BasicInteractive.h"
#include "WindowBI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AWindowBI : public ABasicInteractive
{
	GENERATED_BODY()


public:

	AWindowBI();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Window Interactive", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ObjectSpawner;

	UPROPERTY(Replicated)
	bool bObjectSpawned = false;

	UPROPERTY(Replicated)
	FName ObjectSpawnedID;

	UPROPERTY(Replicated)
	UStaticMesh* SpawnedObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LendObjectMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BorrowObjectMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NoObjectMessage;


protected:

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoBorrowObject();

	void DoBorrowObject();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoLendObject(const FName& ObjectID);

	void DoLendObject(const FName& ObjectID);

public:

	/////// IInteractiveInterface IMPLEMENTATION /////////

	virtual void StartInteract(APawn* Instigator) override;

	/////// IInteractiveInterface IMPLEMENTATION /////////	
		
};
