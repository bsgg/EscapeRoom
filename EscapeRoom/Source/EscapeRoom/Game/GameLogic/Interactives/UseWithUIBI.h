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
class ESCAPEROOM_API AUseWithUIBI : public ABasicInteractive/*, public IUIBasicInteractiveInterface*/
{
	GENERATED_BODY()

public:
	AUseWithUIBI();

protected:

	virtual void BeginPlay() override;

	//UPROPERTY(EditDefaultsOnly, Category = "Use With UI Interactive Settings")
	//bool bDoActionOnThisInteractive = false;

	//UPROPERTY(Replicated, EditDefaultsOnly, Category = "Use With UI Interactive Settings")
	//FName ConnectedInteractiveID;

	UPROPERTY(EditDefaultsOnly, Category = "Use With UI Interactive Settings")
	FString CompletedActionMessage;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Use With UI Interactive Settings")
	bool IsCompleted = false;

public:

	/////// IInteractiveInterface IMPLEMENTATION /////////

	virtual void StartInteract(APawn* PawnInstigator) override;

	/////// IInteractiveInterface IMPLEMENTATION /////////	

	/////// IUIBasicInteractiveInterface IMPLEMENTATION /////////

	virtual void OnComplete() override;

	/////// IUIBasicInteractiveInterface IMPLEMENTATION /////////

protected:

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoAction();

	void DoAction();
	
};
