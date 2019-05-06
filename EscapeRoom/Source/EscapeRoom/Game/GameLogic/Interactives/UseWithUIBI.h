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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Use Settings", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(ReplicatedUsing = OnRep_PickupActionChanged, EditDefaultsOnly, Category = "Use With UI Interactive Settings")
	FActionDefinition PickupAction;

	UPROPERTY(EditDefaultsOnly, Category = "Use With UI Interactive Settings")
	FString CompletedActionMessage;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Use With UI Interactive Settings")
	bool IsCompleted = false;


	UFUNCTION()
	void OnRep_PickupActionChanged();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoPickupAction();

	void DoPickupAction();

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
