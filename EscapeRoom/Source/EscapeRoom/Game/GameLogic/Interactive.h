// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/InteractiveBase.h"
#include "Interactive.generated.h"


UCLASS()
class ESCAPEROOM_API AInteractive : public AInteractiveBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AInputIconMesh;
	
public:	
	AInteractive();

	FORCEINLINE bool GetIsActive() const { return IsActive; }

	FORCEINLINE FName GetObjectID() const { return ObjectID; }

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_DefinitionChanged(FInteractiveDefinition PreviousData) override;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Interactive Settings")
	bool IsActive = true;

	UPROPERTY(ReplicatedUsing = OnRep_ObjectChanged, EditDefaultsOnly, Category = "Interactive Settings")
	FName ObjectID;

	UFUNCTION()
	virtual void OnRep_ObjectChanged();

};
