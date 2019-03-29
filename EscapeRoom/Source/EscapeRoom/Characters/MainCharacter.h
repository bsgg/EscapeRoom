// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utils/Definitions.h"
#include "Game/GameLogic/Interactives/Interact.h"
#include "Game/GameLogic/Interactives/InteractiveInterface.h"
#include "MainCharacter.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOverlappedInteractive, AMainCharacter*, Char, AInteractiveBase*, Interactive);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUIMessageUpdated, AMainCharacter*, Char, FString, Text, bool, HideMessages);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddItemToInventory, AMainCharacter*, Char, FObjectInteraction, Object);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoveItemInventory, AMainCharacter*, Char, FName, ObjectID);

UCLASS()
class ESCAPEROOM_API AMainCharacter : public ACharacter, public IInteract
{
	GENERATED_BODY()

		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UInventoryComponent* InventoryComponent;

public:
	AMainCharacter();

	/** Returns TopDownCameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//FORCEINLINE class UInventoryComponent* GetInventory() const { return InventoryComponent; }	

	/** Returns Gesture **/
	FORCEINLINE EGestureType GetCurrentGesture() const { return CurrentGesture; };

	//// INTERFACE IInteract IMPLEMENTATION ////////////////////
	virtual void NotifyInInteractRange(AActor* Interactive) override;

	virtual void NotifyLeaveInteractRange(AActor* Interactive) override;
	//// INTERFACE IInteract IMPLEMENTATION ////////////////////






	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnOverlappedInteractive OnOverlappedInteractive;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnUIMessageUpdated OnUIMessageUpdated;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnAddItemToInventory OnAddItemToInventory;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnRemoveItemInventory OnRemoveItemInventory;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void LockInput();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UnLockInput();

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void MoveRight(float Value);

	//UFUNCTION(BlueprintCallable, Category = "Input Actions")
	//void OnInteract(FName SelectedObject);

	//UFUNCTION(BlueprintCallable, Category = "Input Actions")
	//void OnInspect();

	//UFUNCTION(BlueprintCallable, Category = "Input Actions")
	//FORCEINLINE class AInteractiveBase* GetInteractive() const { return OverlappedInteractive; }


protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	

	
	

	//void DoInspectAction();

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPCInspectAction();
	// Inspect Action

	// Interact Action
	//void DoInteractAction(FName SelectedObject);

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPCInteractAction(const FName& SelectedObject);
	// Interact Action

	//bool TryToAddNewObject(FName ObjID);



	void SetGestureToDefault();

public:	
	UFUNCTION(Server, Reliable, WithValidation)
	void StartGesture(EGestureType NewGesture);
	
	virtual void Tick(float DeltaTime) override;

	//void OnOverlapInteractive(class AInteractiveBase* Interactive);



protected:

	//UPROPERTY(Replicated)
	//class AInteractiveBase* OverlappedInteractive;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gestures")
	EGestureType CurrentGesture = EGestureType::VE_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gestures")
	float InteractAnimationTime = 1.0f;
	
	FTimerHandle InteractionTimerHandle;	

	bool bInputLocked = false;


private:

	//// INTERFACE IInteract IMPLEMENTATION ////////////////////

	IInteractiveInterface* CurrentInteractive = nullptr;

	void HandleInspectInput();

	void HandleInteractInput();	

	//// INTERFACE IInteract IMPLEMENTATION ////////////////////
};
