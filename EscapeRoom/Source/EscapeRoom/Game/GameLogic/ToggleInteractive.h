// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/InteractiveBase.h"
#include "ToggleInteractive.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AToggleInteractive : public AInteractiveBase
{
	GENERATED_BODY()

public:
	AToggleInteractive();

	void Toggle();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ObjectToToggle;

	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_IsOnChanged, EditDefaultsOnly, Category = "Interactive Settings")
	bool IsOn;

	UFUNCTION()
	void OnRep_IsOnChanged();
	
};
