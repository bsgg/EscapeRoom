// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveInterface.h"
#include "Utils/Definitions.h"
#include "TestInteractive.generated.h"

UCLASS()
class ESCAPEROOM_API ATestInteractive : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestInteractive();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HighlightMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void DestroyPickup();

public:	

	virtual FString GetInteractMessage() override;

	virtual void Inspect(APawn* Instigator)  override;

	// Start execution of the interactive, Add items to inventory, bring up UI
	virtual void StartInteract(APawn* Instigator) override;

	// Finish execution of the interactive, closes UI, destroy objects, etc
	virtual void FinishInteract(APawn* Instigator) override;


protected:

	class AMainCharacter* CharacterOverlapping = nullptr;

};
