// Fill out your copyright notice in the Description page of Project Settings.

#include "TestInteractive.h"
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
	
}


void ATestInteractive::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

}

void ATestInteractive::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


FString ATestInteractive::GetInteractMessage()
{
	return FString(TEXT("Inspect"));
}

// Start execution of the interactive, Add items to inventory, bring up UI
void ATestInteractive::StartInteract(APawn* Instigator)
{
	auto PlayerController = Cast<ALobbyPlayerController>(Instigator->GetController());

	if (PlayerController)
	{
		// SHOW DEBUG WITH A TEXT
	}

}

// Finish execution of the interactive, closes UI, destroy objects, etc
void ATestInteractive::FinishInteract(APawn* Instigator) 
{
	auto PlayerController = Cast<ALobbyPlayerController>(Instigator->GetController());

	if (PlayerController)
	{
		// SHOW DEBUG WITH A TEXT
	}
}



void ATestInteractive::DestroyPickup_Implementation()
{
	Destroy();
}

bool ATestInteractive::DestroyPickup_Validate()
{
	return true;
}