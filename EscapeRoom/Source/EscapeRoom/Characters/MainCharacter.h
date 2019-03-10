// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utils/Definitions.h"
#include "MainCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUIMessageUpdated, AMainCharacter*, Char, FString, Text, bool, HideMessages);


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

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUIMessageUpdated OnUIMessageUpdated;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void LockInput();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UnLockInput();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE bool IsInventoryActive() const { return bInventoryActive; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryActive(bool Active);

	UFUNCTION(BlueprintCallable, Category = "Events")
	TArray<FObjectInteraction> GetObjectsInInventory() const;


	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void OnInteract();

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void TryCombineObjects(const FObjectInteraction& ObjectA, const FObjectInteraction& ObjectB);


protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	

	
	// Inspect Action
	void OnInspect();

	void DoInspectAction();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCInspectAction();
	// Inspect Action

	// Interact Action


	void DoInteractAction();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCInteractAction();
	// Interact Action

	
	UFUNCTION()
	void OnInventoryChanged(class UInventoryComponent* InventoryComp, FName ObjectID, int32 NumberObjects);	

	bool TryToAddNewObject(FName ObjID);

	void StartGesture(EGestureType NewGesture);

	void SetGestureToDefault();

public:	
	virtual void Tick(float DeltaTime) override;

	void OnOverlapInteractive(class AInteractiveBase* Interactive);

protected:

	UPROPERTY(Replicated)
	class AInteractiveBase* OverlappedInteractive;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gestures")
	EGestureType CurrentGesture = EGestureType::VE_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gestures")
	float InteractAnimationTime = 1.0f;
	
	FTimerHandle InteractionTimerHandle;	

	bool bInventoryActive = false;

	bool bInputLocked = false;
};
