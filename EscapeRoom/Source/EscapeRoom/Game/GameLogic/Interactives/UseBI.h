// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/BasicInteractive.h"
#include "UseBI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AUseBI : public ABasicInteractive
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Use Settings", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

	UFUNCTION(BlueprintImplementableEvent)
	void OnUseEvent();

public:
	AUseBI();

protected:
	
	UPROPERTY(ReplicatedUsing = OnRep_UseActionChanged, EditDefaultsOnly, Category = "Use Interactive Settings")
	FActionDefinition UseAction;

	FTimerHandle AnimationTimerHandle;
	void OnEndAnimationTimer();

	UFUNCTION()
	void OnRep_UseActionChanged();

	UPROPERTY(ReplicatedUsing = OnRep_PickupActionChanged, EditDefaultsOnly, Category = "Use Interactive Settings")
	FActionDefinition PickupAction;

	UFUNCTION()
	void OnRep_PickupActionChanged();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoUseAction();

	void DoUseAction();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoPickupAction();

	void DoPickupAction();

public:

	/////// IInteractiveInterface IMPLEMENTATION /////////

	virtual void StartInteract(APawn* Instigator) override;

	/////// IInteractiveInterface IMPLEMENTATION /////////	
	
};
