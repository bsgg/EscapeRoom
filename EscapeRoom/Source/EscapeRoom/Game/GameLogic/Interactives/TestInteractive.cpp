// Fill out your copyright notice in the Description page of Project Settings.

#include "TestInteractive.h"
#include "Characters/MainCharacter.h"
#include "Lobby/LobbyPlayerController.h"

// Sets default values
ATestInteractive::ATestInteractive()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Mesh->SetupAttachment(RootComponent);


	ConstructorHelpers::FObjectFinder<UStaticMesh> highlightAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> highlightMaterial(TEXT("/Game/Common/Materials/HighlightMat"));

	HighlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightMesh"));
	HighlightMesh->SetupAttachment(RootComponent);

	if (highlightAsset.Succeeded())
	{
		HighlightMesh->SetStaticMesh(highlightAsset.Object);
		HighlightMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
		HighlightMesh->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
		HighlightMesh->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));

		if (highlightMaterial.Succeeded())
		{
			HighlightMesh->SetMaterial(0, highlightMaterial.Object);
		}
	}

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ATestInteractive::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ATestInteractive::EndOverlap);


	SetReplicates(true);

}

// Called when the game starts or when spawned
void ATestInteractive::BeginPlay()
{
	Super::BeginPlay();

	HighlightMesh->SetVisibility(false, true);
	
}


void ATestInteractive::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (CharacterOverlapping != nullptr) return;	

	auto MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		CharacterOverlapping = MainCharacter;

		IInteract* Interface = Cast<IInteract>(MainCharacter);

		if (Interface)
		{
			SetOwner(MainCharacter);

			Interface->NotifyInInteractRange(this);

			HighlightMesh->SetVisibility(true, true);
			
		}
	}
}

void ATestInteractive::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		if ((CharacterOverlapping != nullptr) && (CharacterOverlapping == MainCharacter))
		{

			CharacterOverlapping = nullptr; 
			IInteract* Interface = Cast<IInteract>(MainCharacter);

			if (Interface)
			{
				SetOwner(nullptr);

				Interface->NotifyLeaveInteractRange(this);

				HighlightMesh->SetVisibility(false, true);
			}
		}
	}
}


FString ATestInteractive::GetInteractMessage()
{
	return FString(TEXT("Inspect"));
}

void ATestInteractive::Inspect(APawn* Instigator)
{

}

// Start execution of the interactive, Add items to inventory, bring up UI
void ATestInteractive::StartInteract(APawn* Instigator)
{
	auto PlayerController = Cast<ALobbyPlayerController>(Instigator->GetController());

	if (PlayerController)
	{
		// SHOW DEBUG WITH A TEXT

		PlayerController->ShowDebugLog("Start Interact With This Interactive");

		DestroyPickup();
	}

}

// Finish execution of the interactive, closes UI, destroy objects, etc
void ATestInteractive::FinishInteract(APawn* Instigator) 
{
	auto PlayerController = Cast<ALobbyPlayerController>(Instigator->GetController());

	if (PlayerController)
	{
		// HIDE DEBUG WITH A TEXT

		PlayerController->ShowDebugLog("Finish Interacting");
	}
}

FString ATestInteractive::GetInteractID()
{
	return FString(TEXT("None"));
}



void ATestInteractive::DestroyPickup_Implementation()
{
	Destroy();
}

bool ATestInteractive::DestroyPickup_Validate()
{
	return true;
}