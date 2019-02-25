// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utils/Definitions.h"
#include "MainCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIMessageUpdated, AMainCharacter*, Char, FString, Text);


UCLASS()
class ESCAPEROOM_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

public:
	AMainCharacter();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UInventoryComponent* GetInventory() const { return InventoryComponent; }	

	/** Returns Gesture **/
	FORCEINLINE EGestureType GetCurrentGesture() const { return CurrentGesture; };

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GamePlay")
	bool bIsCarryingObjective = false;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUIMessageUpdated OnUIMessageUpdated;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	
	// Inspect Action
	void OnInspect();

	void DoInspectAction();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCInspectAction();
	// Inspect Action

	// Interact Action
	void OnInteract();

	void DoInteractAction();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCInteractAction();
	// Interact Action



	void Interact();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract();

	UFUNCTION()
	void OnInventoryChanged(class UInventoryComponent* InventoryComp, FName ObjectID, int32 NumberObjects);	

	void StartGesture(EGestureType NewGesture);

	void SetGestureToDefault();

public:	
	virtual void Tick(float DeltaTime) override;

	void OnOverlapPickup(class APickup* Pickup, FName ObjectID);	

	void OnOverlapInteractable(class AInteractable* Interactable);

	void OnOverlapInteractive(class AInteractive* Interactive);

protected:

	UPROPERTY(Replicated)
	class APickup* OnOverlappedPickup;

	UPROPERTY(Replicated)
	class AInteractable* OverlappedInteractable;

	UPROPERTY(Replicated)
	class AInteractive* OverlappedInteractive;

	

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gestures")
	EGestureType CurrentGesture = EGestureType::VE_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gestures")
	float InteractAnimationTime = 1.0f;
	
	FTimerHandle InteractionTimerHandle;	
};
