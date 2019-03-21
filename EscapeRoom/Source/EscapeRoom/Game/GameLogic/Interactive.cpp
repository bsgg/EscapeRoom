// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactive.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "UObject/ConstructorHelpers.h"



#include "UnrealNetwork.h"


// Sets default values
AInteractive::AInteractive()
{
	// Icon
	//ConstructorHelpers::FObjectFinder<UStaticMesh> iconAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));

	//ConstructorHelpers::FObjectFinder<UMaterialInterface> AInputIconMaterial(TEXT("/Game/Game/GameLogic/Interactives/Materials/M_AInputControl"));

	//AInputIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AInputIconMesh"));
	//AInputIconMesh->SetupAttachment(RootComponent);

	//if (iconAsset.Succeeded())
	//{
		/*AInputIconMesh->SetStaticMesh(iconAsset.Object);
		AInputIconMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		AInputIconMesh->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
		AInputIconMesh->SetWorldScale3D(FVector(0.4f, 0.4f, 0.04f));

		if (AInputIconMaterial.Succeeded())
		{
			AInputIconMesh->SetMaterial(0, AInputIconMaterial.Object);
		}

	}*/

	SetReplicates(true);
}


void AInteractive::BeginPlay()
{
	Super::BeginPlay();

	//AInputIconMesh->SetVisibility(false);
}


