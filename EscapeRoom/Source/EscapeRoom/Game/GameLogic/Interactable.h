// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Definitions.h"
#include "Interactable.generated.h"

UCLASS()
class ESCAPEROOM_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();

	FORCEINLINE FInteractionData GetData() const { return Data; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ObjectMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UInteractiveUIComponent* UI;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(Replicated)
	class UInteractableUI* Widget;

	UPROPERTY(Replicated)
	FInteractionData OldData;

	UPROPERTY(ReplicatedUsing = OnRep_DataChanged, EditAnywhere, BlueprintReadWrite, Category = "Interactable Data")
	FInteractionData Data;	

	UPROPERTY(Replicated)
	class AMainCharacter* CharacterOverlapping;

	UFUNCTION()
	void OnRep_DataChanged(FInteractionData PreviousData);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// View action (primary action)
	FString GetViewDescription() const;

	void AdvanceViewDescription();

	void ResetViewDescription();


	// Interaciton Action (secondary action)
	void EnableSecondaryAction();
	void DisableSecondaryAction();
	bool HasSecondaryActionObject();
	FName GetSecondaryActionObjectID();
	void RemoveSecondaryActionObject();

};
