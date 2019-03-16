// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "CharacterAnimator.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Game/Components/InventoryComponent.h"

#include "Game/GameLogic/InteractiveBase.h"
#include "Game/GameLogic/PickupInteractive.h"
#include "Game/GameLogic/SwitchInteractive.h" 
#include "Game/GameLogic/UseInteractive.h"

//#include "Game/Pickup.h"
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
	//InventoryComponent->OnInventoryChanged.AddDynamic(this, &AMainCharacter::OnInventoryChanged);

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

	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &AMainCharacter::OnInspect);
	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::OnInteract);
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
	if ((CurrentGesture != EGestureType::VE_NONE) || (bInputLocked)) return;

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

// REGION INVENTORY
//void AMainCharacter::SetInventoryActive(bool Active)
//{
	//bInventoryActive = Active;
//}

//TArray<FObjectInteraction> AMainCharacter::GetObjectsInInventory() const
//{
	//return InventoryComponent->GetObjects();
//}
// ENDREGION INVENTORY


// REGION INSPECT ACTION
void AMainCharacter::OnInspect()
{
	//if (OverlappedInteractable == nullptr) return;
	if (OverlappedInteractive == nullptr) return;

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
	if (OverlappedInteractive == nullptr) return;

	FString desc = OverlappedInteractive->GetInspectDetail();

	OverlappedInteractive->ForwardInspectDetail();

	OnUIMessageUpdated.Broadcast(this, desc, false);
}
// ENDREGION INSPECT ACTION


// REGION INTERACT ACTION
void AMainCharacter::OnInteract(FName SelectedObject)
{	

	

	UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::OnInteract] SelectedObject: %s"), *SelectedObject.ToString());

	if (bInputLocked) return;

	if (Role < ROLE_Authority)
	{
		ServerRPCInteractAction(SelectedObject);
	}
	else
	{
		DoInteractAction(SelectedObject);
	}
}

bool AMainCharacter::TryToAddNewObject(FName ObjID)
{
	ARoomGameMode* GM = Cast<ARoomGameMode>(GetWorld()->GetAuthGameMode());

	FObjectInteraction* NewObject = GM->GetObjectByID(ObjID);

	if (NewObject == nullptr) return false;

	InventoryComponent->AddObject(ObjID, *NewObject);

	TArray<FObjectInteraction> Objects;
	Objects.Add(*NewObject);

	OnAddItemToInventory.Broadcast(this, *NewObject);

	return true;
}

void AMainCharacter::DoInteractAction(FName SelectedObject)
{
	UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] SelectedObject: %s"), *SelectedObject.ToString());

	//  Check type action
	if (OverlappedInteractive == nullptr) return;

	APickupInteractive* Pickup = Cast<APickupInteractive>(OverlappedInteractive);
	if (Pickup != nullptr)
	{
		if ((Pickup->GetPickupAction().IsActive) && (Pickup->GetPickupAction().HasObject()))
		{
			FName ObjectID = Pickup->GetPickupAction().ObjectID;

			Pickup->PickupObject();

			if (TryToAddNewObject(ObjectID))
			{
				FString desc = Pickup->GetPickupAction().DetailDefaultAction.ToString();

				OnUIMessageUpdated.Broadcast(this, desc, false);

				StartGesture(EGestureType::VE_INTERACT);
			}			
		}	
		return;
	}	


	ASwitchInteractive* Switch = Cast<ASwitchInteractive>(OverlappedInteractive);
	if (Switch != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] It is an interactive  Switch %s"), *Switch->GetName());

		Switch->Toggle();

		StartGesture(EGestureType::VE_INTERACT);

		return;
	}


	AUseInteractive* UseInteractive = Cast<AUseInteractive>(OverlappedInteractive);
	if (UseInteractive != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] %s is UseInteractive"), *UseInteractive->GetName());
		if (UseInteractive->GetUseAction().IsActive)
		{
			//UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] UseInteractive UseAction Active"));

			if (UseInteractive->GetUseAction().HasObject())
			{
				// Check if the player has the object on the inventory



				if (SelectedObject == UseInteractive->GetUseAction().ObjectID)
				//UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] UseInteractive, Needs an object"));
				//if (InventoryComponent->CheckIfObjectExists(UseInteractive->GetUseAction().ObjectID))
				{

					UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] UseInteractive I have the object: %s"),*UseInteractive->GetUseAction().ObjectID.ToString());

					//UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] UseInteractive, Object exists in inventory"));

					// Remove object from inventory
					InventoryComponent->RemoveObject(UseInteractive->GetUseAction().ObjectID);

					//TODO: REMOVE THE OBJECT VISUALLY

					UseInteractive->Use();

					StartGesture(EGestureType::VE_INTERACT);

					FString desc = UseInteractive->GetUseAction().DetailDefaultAction.ToString();

					OnUIMessageUpdated.Broadcast(this, desc, false);

				}
				else
				{
					FString desc = UseInteractive->GetUseAction().DetailWrongAction.ToString();

					OnUIMessageUpdated.Broadcast(this, desc, false);

					StartGesture(EGestureType::VE_DISMISS);
				}				
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] UseInteractive, Doesn't need an object"));

				UseInteractive->Use();

				StartGesture(EGestureType::VE_INTERACT);

				FString desc = UseInteractive->GetUseAction().DetailDefaultAction.ToString();

				OnUIMessageUpdated.Broadcast(this, desc, false);
			}

		}
		else if ((UseInteractive->GetPickupAction().IsActive) && (UseInteractive->GetPickupAction().HasObject()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::DoInteractAction] UseInteractive, PickupAction Is Active And Has Object"));

			FName ObjectID = UseInteractive->GetPickupAction().ObjectID;

			UseInteractive->Pickup();

			if (TryToAddNewObject(ObjectID))
			{
				StartGesture(EGestureType::VE_INTERACT);

				FString desc = UseInteractive->GetPickupAction().DetailDefaultAction.ToString();

				OnUIMessageUpdated.Broadcast(this, desc, false);
			}
		}

		return;
	}
}

void AMainCharacter::ServerRPCInteractAction_Implementation(const FName& SelectedObject)
{
	UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::ServerRPCInteractAction] SelectedObject: %s"), *SelectedObject.ToString());

	DoInteractAction(SelectedObject);
}

bool AMainCharacter::ServerRPCInteractAction_Validate(const FName& SelectedObject)
{
	return true;
}
// ENDREGION INTERACT ACTION

//void AMainCharacter::TryCombineObjects(const FObjectInteraction& ObjectA, const FObjectInteraction& ObjectB)
//{

//}


void AMainCharacter::OnOverlapInteractive(class AInteractiveBase* Interactive)
{
	if (Role == ROLE_Authority)
	{
		OverlappedInteractive = Interactive;

		if (OverlappedInteractive == nullptr)
		{
			OnUIMessageUpdated.Broadcast(this, "", true);
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

/*
void AMainCharacter::OnInventoryChanged(class UInventoryComponent* InventoryComp, FName ObjectID, int32 NumberObjects)
{
	UE_LOG(LogTemp, Warning, TEXT("[AMainCharacter::OnInventoryChanged] ObjectID: %s"), *ObjectID.ToString());
}
*/


void AMainCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacter, CurrentGesture);

	//DOREPLIFETIME(AMainCharacter, SelectedObjectInventory);	

	DOREPLIFETIME(AMainCharacter, OverlappedInteractive);
}



