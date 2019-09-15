// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "CharacterAnimator.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Lobby/LobbyPlayerController.h"

#include "Materials/Material.h"
#include "Engine/World.h"

#include "UnrealNetwork.h"

// Sets default values
AMainCharacter::AMainCharacter()
{  
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);   

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;  
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false; 

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("/Game/Common/Materials/M_Cursor_Decal"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());


	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Unlock input
	if (bInputLocked)
	{
		ElpasedLockedInput += DeltaTime;
		if (ElpasedLockedInput >= WaitTimeToUnlockInput)
		{
			ElpasedLockedInput = 0.0f;

			bInputLocked = false;
		}
	}

	// Wait to set animation to default
	if (bSetAnimationToDefault)
	{
		ElpasedAnimationToDefault += DeltaTime;
		if (ElpasedAnimationToDefault >= WaitAnimationToDefault)
		{
			ElpasedAnimationToDefault = 0.0f;

			bSetAnimationToDefault = false;

			CurrentGesture = EGestureType::VE_NONE;
		}
	}

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::LockInput()
{
	bInputLocked = true;
}

void AMainCharacter::UnLockInput()
{
	bInputLocked = false;
}

void AMainCharacter::MoveForward(float Value)
{
	if ((CurrentGesture != EGestureType::VE_NONE) || (bInputLocked)) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{

	if ((CurrentGesture != EGestureType::VE_NONE) || (bInputLocked)) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


//// INTERFACE IInteract IMPLEMENTATION ////////////////////

void AMainCharacter::NotifyInInteractRange(AActor* Interactive)
{
	if (IsLocallyControlled())
	{
		CurrentInteractive = CastChecked<IInteractiveInterface>(Interactive);
		if (CurrentInteractive)
		{
			auto PlayerController = CastChecked<ALobbyPlayerController>(GetController());

			FString Debug = "Enter in Interactive " + CurrentInteractive->GetInteractID();
			PlayerController->ShowDebugLog(Debug);

			// Show Controls
			PlayerController->ShowControls();
		}
	}
}

void AMainCharacter::NotifyLeaveInteractRange(AActor* Interactive)
{
	if (IsLocallyControlled())
	{		
		auto PlayerController = CastChecked<ALobbyPlayerController>(GetController());

		FString Debug = "Leaving Interactive " + CurrentInteractive->GetInteractID();

		PlayerController->ShowDebugLog(Debug);

		CurrentInteractive = nullptr;

		// Hide Controls
		PlayerController->HideControls();	
		PlayerController->HideMessage();
	}
}
//// INTERFACE IInteract IMPLEMENTATION ////////////////////


//// INPUT IMPLEMENTATION ////////////////////

void AMainCharacter::HandleInspectInput()
{
	if ((CurrentGesture != EGestureType::VE_NONE) || (bInputLocked)) return;

	if (IsLocallyControlled() && CurrentInteractive)
	{
		CurrentInteractive->Inspect(this);
	}
}

void AMainCharacter::HandleInteractInput()
{
	if ((CurrentGesture != EGestureType::VE_NONE) || (bInputLocked)) return;

	if (IsLocallyControlled() && CurrentInteractive)
	{
		CurrentInteractive->StartInteract(this);
	}
}

//// INPUT IMPLEMENTATION ////////////////////

void AMainCharacter::StartGesture_Implementation(const EGestureType& NewGesture, float lockTime)
{
	bInputLocked = true;
	CurrentGesture = NewGesture;

	ElpasedLockedInput = 0.0f;
	WaitTimeToUnlockInput = lockTime;

	// Set new animation
	if (NewGesture != EGestureType::VE_NONE)
	{
		WaitAnimationToDefault = 1.0f;
		switch (NewGesture)
		{
			case EGestureType::VE_DISMISS:
				WaitAnimationToDefault = DismissAnimationTime;
			break;
			case EGestureType::VE_INSPECT:
				WaitAnimationToDefault = InspectAnimationTime;
			break;
			case EGestureType::VE_INTERACT:
				WaitAnimationToDefault = InteractAnimationTime;
			break;
		}
		
		ElpasedAnimationToDefault = 0.0f;
		bSetAnimationToDefault = true;

	}
}

bool AMainCharacter::StartGesture_Validate(const EGestureType& NewGesture, float lockTime)
{
	return true;
}

void AMainCharacter::SetGestureToDefault()
{
	CurrentGesture = EGestureType::VE_NONE;
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacter, CurrentGesture);
}



