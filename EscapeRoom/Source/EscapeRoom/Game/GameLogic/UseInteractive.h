// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactive.h"
#include "Utils/Definitions.h"
#include "UseInteractive.generated.h"

USTRUCT(BlueprintType)
struct FActionDefinition
{
	GENERATED_BODY()

public:

	FActionDefinition()
	{
		ObjectID = "NONE";
		AnimationLength = 0.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DetailDefaultAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DetailWrongAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimationLength;

	bool HasObject()
	{
		return (ObjectID.ToString().ToLower() != "none");
	}

	bool HasAnimation()
	{
		return (AnimationLength > 0.0f);
	}
};


/**
 * Use Interactive: An Interactive of this type, could need an object to be enabled. 
 * After is the use action is enabled, the pickup action is revealed.
 */
UCLASS()
class ESCAPEROOM_API AUseInteractive : public AInteractive
{
	GENERATED_BODY()

public:
	
	AUseInteractive();

	// Use Action
	FORCEINLINE FActionDefinition GetUseAction() const { return UseAction; }

	FORCEINLINE FActionDefinition GetPickupAction() const { return PickupAction; }

	void Use();	

	UFUNCTION(BlueprintImplementableEvent)
	void OnUseEvent();


	void Pickup();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Use Interactive Settings", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

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




};
