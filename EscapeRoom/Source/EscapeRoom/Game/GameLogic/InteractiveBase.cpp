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
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveBase::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AInteractiveBase::EndOverlap);

	SetReplicates(true);

}

// Called when the game starts or when spawned
void AInteractiveBase::BeginPlay()
{
	Super::BeginPlay();

	HighlightMesh->SetVisibility(false, true);	
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

			//CharacterOverlapping->OnOverlapInteractive(this);

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
			//CharacterOverlapping->OnOverlapInteractive(nullptr);

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
		HighlightMesh->SetVisibility(true, true);
	}
	else
	{
		HighlightMesh->SetVisibility(false, true);
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

void AInteractiveBase::DisableCollider()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->SetGenerateOverlapEvents(false);
}

void AInteractiveBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveBase, Definition);

	DOREPLIFETIME(AInteractiveBase, OldDefinition);

	DOREPLIFETIME(AInteractiveBase, CharacterOverlapping);
}

