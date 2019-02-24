// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPickupChanged, APickup*, Pickup, FName, ObjectID);

UCLASS()
class ESCAPEROOM_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();

	FORCEINLINE FName GetObjectID() const { return ObjectID; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* OtherMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

	// EditDefaultsOnly Only editable when create the blueprint not runtime
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* OverlappedSound;

	// Replicated variable, when something happens on this variable, OnRep_IsActive will be called (it's an event) only on clients
	// By default not called on the server
	UPROPERTY(ReplicatedUsing=OnRep_IsActive)
	bool bIsActive = true;

	// Only in client
	UFUNCTION()
	void OnRep_IsActive();

	void ShowOtherMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FName ObjectID;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPickupChanged OnPickupChanged;

};
