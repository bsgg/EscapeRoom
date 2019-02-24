// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "CharacterAnimator.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Game/Components/InventoryComponent.h"
#include "Game/GameLogic/Interactable.h"
#include "Game/Pickup.h"
#include "Game/RoomGameMode.h"
#include "Utils/Definitions.h"

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

	// Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->OnInventoryChanged.AddDynamic(this, &AMainCharacter::OnInventoryChanged);

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
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &AMainCharacter::OnInspect);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::OnInteract);
}

void AMainCharacter::MoveForward(float Value)
{
	if (CurrentGesture != EGestureType::VE_NONE) return;

	if (Value != 0.0f)
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
	if (CurrentGesture != EGestureType::VE_NONE) return;

	if (Value != 0.0f)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


// REGION INSPECT ACTION
void AMainCharacter::OnInspect()
{
	if (OverlappedInteractable == nullptr) return;

	if (Role < ROLE_Authority)
	{
		ServerRPCInspectAction();
	}else
	{
		DoInspectAction();
	}
}

void AMainCharacter::ServerRPCInspectAction_Implementation()
{
	DoInspectAction();
}

bool AMainCharacter::ServerRPCInspectAction_Validate()
{
	return true;
}

void AMainCharacter::DoInspectAction()
{
	if (OverlappedInteractable == nullptr) return;

	FString desc = OverlappedInteractable->GetViewDescription();

	OverlappedInteractable->AdvanceViewDescription();

	OnUIMessageUpdated.Broadcast(this, desc);
}
// ENDREGION INSPECT ACTION



// REGION INTERACT ACTION
void AMainCharacter::OnInteract()
{
	if (Role < ROLE_Authority)
	{
		ServerRPCInteractAction();
	}
	else
	{
		DoInteractAction();
	}
}

void AMainCharacter::DoInteractAction()
{
	//  Check type action
	if ((OverlappedInteractable == nullptr) && (!OverlappedInteractable->GetData().SecondaryAction.Active)) return;

	bool PickupAction = (OverlappedInteractable->GetData().SecondaryAction.InteractionType == EInteractionType::VE_PICKUP);

	if (PickupAction)
	{
		//  Add object to inventory
		if (OverlappedInteractable->HasSecondaryActionObject())
		{
			FName objectID = OverlappedInteractable->GetSecondaryActionObjectID();
			InventoryComponent->AddObject(objectID);
			StartGesture(EGestureType::VE_INTERACT);

			OverlappedInteractable->RemoveSecondaryActionObject();
		}
		else
		{
			FString desc = OverlappedInteractable->GetData().SecondaryAction.GetRandomDescription(false).ToString();
			OnUIMessageUpdated.Broadcast(this, desc);
		}		
	}   

}

void AMainCharacter::ServerRPCInteractAction_Implementation()
{
	DoInteractAction();
}

bool AMainCharacter::ServerRPCInteractAction_Validate()
{
	return true;
}
// ENDREGION INTERACT ACTION



void AMainCharacter::OnOverlapInteractable(class AInteractable* Interactable)
{
	if (Role == ROLE_Authority)
	{
		OverlappedInteractable = Interactable;		

		if (OverlappedInteractable == nullptr)
		{
			OnUIMessageUpdated.Broadcast(this, "");
		}
	}
	
}

void AMainCharacter::StartGesture(EGestureType NewGesture)
{
	CurrentGesture = NewGesture;

	if (NewGesture != EGestureType::VE_NONE)
	{
		GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, this, &AMainCharacter::SetGestureToDefault, InteractAnimationTime, false);
	}
}


void AMainCharacter::SetGestureToDefault()
{

	CurrentGesture = EGestureType::VE_NONE;
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
}








// OLD


void AMainCharacter::Interact()
{
	if ((OverlappedInteractable == nullptr) || (InventoryComponent == nullptr)) return;
	if (Role < ROLE_Authority)
	{
		ServerInteract();
	}





	//UWorld* World = GetWorld();
	//if (World == nullptr) return;
	//ARoomGameMode* GameMode = Cast<ARoomGameMode>(World->GetAuthGameMode());

	//if (GameMode == nullptr) 
	//{
		//UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::Interact] GameMode Null"));
		//return;
	//}


	//if ((OnOverlappedPickup == nullptr) || (InventoryComponent == nullptr)) return;

	// If not server, make call ServerFunction, so it will make a request to the server, and the server will
	// Run the function (THE CLIENT ONLY MAKES THE REQUEST)
	//if (Role < ROLE_Authority)
	//{
		//ServerInteract();
	//}	


	// Check the overlapped pickup

	// Add Object to inventory
	//UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::Interact] Trying to add.. %s"), *OnOverlappedPickup->GetObjectID().ToString());
	//InventoryComponent->AddObject(OnOverlappedPickup->GetObjectID());

	// Set animation to interaction
	//StartGesture(EGestureType::VE_INTERACT);
}

void AMainCharacter::ServerInteract_Implementation()
{
	Interact();
}

bool AMainCharacter::ServerInteract_Validate()
{
	return true;
}



void AMainCharacter::OnInventoryChanged(class UInventoryComponent* InventoryComp, FName ObjectID, int32 NumberObjects)
{
	UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::OnInventoryChanged] ObjectID: %s"), *ObjectID.ToString());
}


void AMainCharacter::OnOverlapPickup(class APickup* Pickup, FName ObjectID)
{
	if (Role == ROLE_Authority)
	{
		OnOverlappedPickup = Pickup;
	}
}



void AMainCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacter, bIsCarryingObjective);

	DOREPLIFETIME(AMainCharacter, CurrentGesture);

	DOREPLIFETIME(AMainCharacter, OnOverlappedPickup);

	DOREPLIFETIME(AMainCharacter, OverlappedInteractable);
	
}



