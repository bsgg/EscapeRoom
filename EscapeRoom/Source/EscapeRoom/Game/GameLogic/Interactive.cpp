// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactive.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "UObject/ConstructorHelpers.h"

#include "Characters/MainCharacter.h"

#include "UnrealNetwork.h"


// Sets default values
AInteractive::AInteractive()
{
	// Set pickup mesh as a root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Mesh->SetupAttachment(RootComponent);

	// Icons
	ConstructorHelpers::FObjectFinder<UStaticMesh> iconAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));

	ConstructorHelpers::FObjectFinder<UMaterialInterface> AInputIconMaterial(TEXT("/Game/Game/GameLogic/Interactives/Materials/M_AInputControl"));

	ConstructorHelpers::FObjectFinder<UMaterialInterface> XInputIconMaterial(TEXT("/Game/Game/GameLogic/Interactives/Materials/M_XInputControl"));
	
	AInputIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AInputIconMesh"));
	AInputIconMesh->SetupAttachment(RootComponent);

	XInputIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XInputIconMesh"));
	XInputIconMesh->SetupAttachment(RootComponent);

	if (iconAsset.Succeeded())
	{
		AInputIconMesh->SetStaticMesh(iconAsset.Object);
		AInputIconMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		AInputIconMesh->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
		AInputIconMesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.02f));

		if (AInputIconMaterial.Succeeded())
		{
			AInputIconMesh->SetMaterial(0,AInputIconMaterial.Object);
		}

		XInputIconMesh->SetStaticMesh(iconAsset.Object);
		XInputIconMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		XInputIconMesh->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
		XInputIconMesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.02f));

		if (XInputIconMaterial.Succeeded())
		{
			XInputIconMesh->SetMaterial(0, XInputIconMaterial.Object);
		}
	}
	
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AInteractive::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AInteractive::EndOverlap);
	
	SetReplicates(true);
}


void AInteractive::BeginPlay()
{
	Super::BeginPlay();

	XInputIconMesh->SetVisibility(false);
	AInputIconMesh->SetVisibility(false);

}

void AInteractive::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (Definition.IsLocked) return;

	if (OtherActor == nullptr) return;

	if (Role == ROLE_Authority)
	{
		if (CharacterOverlapping == nullptr)
		{
			CharacterOverlapping = Cast<AMainCharacter>(OtherActor);
			if (CharacterOverlapping == nullptr) return;
			CharacterOverlapping->OnOverlapInteractive(this);

			OldDefinition.IsLocked = false;

			Definition.IsLocked = true;

			OnRep_DefinitionChanged(OldDefinition);
		}
	}

}

void AInteractive::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;

	if (Role == ROLE_Authority)
	{
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter == nullptr) return;

		// There is a character already overlapping and it's the same as the leaving character
		if ((CharacterOverlapping != nullptr) && (CharacterOverlapping == OtherCharacter))
		{
			CharacterOverlapping->OnOverlapInteractive(nullptr);

			ResetInspectDetail();

			CharacterOverlapping = nullptr;

			Definition.IsLocked = false;

			OnRep_DefinitionChanged(Definition);

		}
	}
}

void AInteractive::OnRep_DefinitionChanged(FInteractiveDefinition PreviousData)
{
	if (Definition.IsLocked)
	{
		XInputIconMesh->SetVisibility(true);
	}
	else
	{
		XInputIconMesh->SetVisibility(false);
	}
}

FString AInteractive::GetInspectDetail() const
{
	if (Definition.IsDefaultDetailInspectActive)
	{
		return Definition.DefaultDetailInspect.ToString();
	}

	if ((Definition.DetailInspect.Num() > 0) && (Definition.IndexDetailInspect >= 0) && (Definition.IndexDetailInspect < Definition.DetailInspect.Num()))
	{
		return Definition.DetailInspect[Definition.IndexDetailInspect].ToString();
	}

	return NONE_TAG.ToString();
}

void AInteractive::ForwardInspectDetail()
{
	Definition.IndexDetailInspect += 1;
	if (Definition.IndexDetailInspect >= Definition.DetailInspect.Num())
	{
		Definition.IndexDetailInspect = Definition.DetailInspect.Num() - 1;
	}
}

void AInteractive::ResetInspectDetail()
{
	Definition.IndexDetailInspect = 0;
}


void AInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractive, Definition);

	DOREPLIFETIME(AInteractive, OldDefinition);

	DOREPLIFETIME(AInteractive, CharacterOverlapping);
}