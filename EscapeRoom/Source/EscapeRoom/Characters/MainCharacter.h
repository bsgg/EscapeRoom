// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utils/Definitions.h"
#include "Game/GameLogic/Interactives/Interact.h"
#include "Game/GameLogic/Interactives/InteractiveInterface.h"
#include "MainCharacter.generated.h"


UCLASS()
class ESCAPEROOM_API AMainCharacter : public ACharacter, public IInteract
{
	GENERATED_BODY()

		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


public:
	AMainCharacter();

	/** Returns Gesture **/
	FORCEINLINE EGestureType GetCurrentGesture() const { return CurrentGesture; };

	//// INTERFACE IInteract IMPLEMENTATION ////////////////////
	virtual void NotifyInInteractRange(AActor* Interactive) override;

	virtual void NotifyLeaveInteractRange(AActor* Interactive) override;
	//// INTERFACE IInteract IMPLEMENTATION ////////////////////
	   
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void LockInput();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UnLockInput();

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input Actions")
	void MoveRight(float Value);


protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void SetGestureToDefault();

public:	

	// Gesture animation, includes times to lock the character
	UFUNCTION(Server, Reliable, WithValidation)
	void StartGesture(const EGestureType& NewGesture, float lockTime);
	
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gestures")
	EGestureType CurrentGesture = EGestureType::VE_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	float InspectAnimationTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	float InteractAnimationTime = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	float DismissAnimationTime = 0.7f;


	bool bSetAnimationToDefault = false;

	float ElpasedAnimationToDefault = 0.0f;

	float WaitAnimationToDefault = 1.0f;

	bool bInputLocked = false;

	float ElpasedLockedInput = 0.0f;

	float WaitTimeToUnlockInput = 1.0f;

private:

	//// INTERFACE IInteract IMPLEMENTATION ////////////////////

	IInteractiveInterface* CurrentInteractive = nullptr;

	void HandleInspectInput();

	void HandleInteractInput();	

	//// INTERFACE IInteract IMPLEMENTATION ////////////////////
};
