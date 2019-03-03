// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Characters/MainCharacter.h"
#include "UnrealNetwork.h"



AInteractiveBase::AInteractiveBase()
{
	// Set pickup mesh as a root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Mesh->SetupAttachment(RootComponent);

	// Icons
	ConstructorHelpers::FObjectFinder<UStaticMesh> iconAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> XInputIconMaterial(TEXT("/Game/Game/GameLogic/Interactives/Materials/M_XInputControl"));

	XInputIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XInputIconMesh"));
	XInputIconMesh->SetupAttachment(RootComponent);

	if (iconAsset.Succeeded())
	{	
		XInputIconMesh->SetStaticMesh(iconAsset.Object);
		XInputIconMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		XInputIconMesh->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
		XInputIconMesh->SetWorldScale3D(FVector(0.4f, 0.4f, 0.04f));

		if (XInputIconMaterial.Succeeded())
		{
			XInputIconMesh->SetMaterial(0, XInputIconMaterial.Object);
		}
	}

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveBase::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AInteractiveBase::EndOverlap);

	SetReplicates(true);

}

// Called when the game starts or when spawned
void AInteractiveBase::BeginPlay()
{
	Super::BeginPlay();

	XInputIconMesh->SetVisibility(false);
	
}


void AInteractiveBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
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

void AInteractiveBase::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void AInteractiveBase::OnRep_DefinitionChanged(FInteractiveDefinition PreviousData)
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

FString AInteractiveBase::GetInspectDetail() const
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

void AInteractiveBase::ForwardInspectDetail()
{
	Definition.IndexDetailInspect += 1;	

	if (Definition.IndexDetailInspect >= Definition.DetailInspect.Num())
	{
		
		Definition.IndexDetailInspect = Definition.DetailInspect.Num() - 1;
	}
}


void AInteractiveBase::ResetInspectDetail()
{
	Definition.IndexDetailInspect = 0;
}


void AInteractiveBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveBase, Definition);

	DOREPLIFETIME(AInteractiveBase, OldDefinition);

	DOREPLIFETIME(AInteractiveBase, CharacterOverlapping);
}

