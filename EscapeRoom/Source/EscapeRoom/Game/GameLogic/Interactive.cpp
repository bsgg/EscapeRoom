// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactive.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

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
	AInputIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AInputIconMesh"));
	AInputIconMesh->SetupAttachment(RootComponent);

	XInputIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XInputIconMesh"));
	XInputIconMesh->SetupAttachment(RootComponent);

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