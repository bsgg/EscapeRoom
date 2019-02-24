// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeZone.generated.h"

UCLASS()
class ESCAPEROOM_API AEscapeZone : public AActor
{
	GENERATED_BODY()
	
public:	

	AEscapeZone();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Componets")
	class UBoxComponent* OverlapComp;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// EditDefaultsOnly Only editable when create the blueprint not runtime
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* CompletedRoomSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	FName ObjectID;

public:	

	virtual void Tick(float DeltaTime) override;

};
