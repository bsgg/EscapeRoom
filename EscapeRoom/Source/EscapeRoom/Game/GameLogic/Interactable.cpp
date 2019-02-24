// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "WidgetComponent.h"
#include "Characters/MainCharacter.h"
#include "Game/UI/InteractableUI.h"
#include "Game/UI/ControlWidget.h"
#include "Game/UI/InteractiveUIComponent.h"
#include "Game/RoomGameMode.h"
#include "UnrealNetwork.h"

// Sets default values
AInteractable::AInteractable()
{
	// Set pickup mesh as a root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AInteractable::EndOverlap);

	// Create widget
	UI = CreateDefaultSubobject<UInteractiveUIComponent>(TEXT("UI"));
	UI->SetWidgetSpace(EWidgetSpace::Screen);
	UI->SetDrawAtDesiredSize(true);
	UI->SetupAttachment(RootComponent);


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);

}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	UI->Initialize();

	UI->GetUI()->GetAControl()->Enable();
	UI->GetUI()->GetXControl()->Enable();

	if (Data.SecondaryAction.InteractionType == EInteractionType::VE_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AInteractable::InitializeWidget] Secondary Option is NONE"));

		UI->GetUI()->GetAControl()->Disable();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AInteractable::InitializeWidget] Secondary Option is OTHER"));

		//UI->GetUI()->GetAControl()->Enable();
	}
}


// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractable::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (Data.Status == EInteractionStatus::VE_LOCKED) return;

	if (OtherActor == nullptr) return;

	if (Role == ROLE_Authority)
	{
		if (CharacterOverlapping == nullptr)
		{
			CharacterOverlapping = Cast<AMainCharacter>(OtherActor);
			if (CharacterOverlapping == nullptr) return;

			ResetViewDescription();

			CharacterOverlapping->OnOverlapInteractable(this);

			Data.Status = EInteractionStatus::VE_LOCKED;

			OnRep_DataChanged();
		}
		
	}
	
}

void AInteractable::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	
	if (Role == ROLE_Authority)
	{
		AMainCharacter* OtherCharacter = Cast<AMainCharacter>(OtherActor);
		if (OtherCharacter == nullptr) return;

		// There is a character already overlapping and it's the same as the leaving character
		if ((CharacterOverlapping != nullptr) && (CharacterOverlapping == OtherCharacter))		
		{
			CharacterOverlapping->OnOverlapInteractable(nullptr);

			ResetViewDescription();

			CharacterOverlapping = nullptr;

			Data.Status = EInteractionStatus::VE_UNLOCKED;

			OnRep_DataChanged();

		}
	}
}


void AInteractable::OnRep_DataChanged()
{	switch (Data.Status)
	{
		case EInteractionStatus::VE_LOCKED:
			UI->GetUI()->GetAControl()->Show();
			UI->GetUI()->GetXControl()->Show();
		break;

		case EInteractionStatus::VE_UNLOCKED:
			UI->GetUI()->GetAControl()->Hide();
			UI->GetUI()->GetXControl()->Hide();
		break;
	}	
}

// REGION VIEW DESCRIPTION
FString AInteractable::GetViewDescription() const
{	
	return (Data.PrimaryAction.GetDescriptionById(Data.PrimaryAction.Index).ToString());
}

void AInteractable::AdvanceViewDescription()
{
	Data.PrimaryAction.Index +=1;
	if (Data.PrimaryAction.Index >= Data.PrimaryAction.DescriptionList.Num())
	{
		Data.PrimaryAction.Index = Data.PrimaryAction.DescriptionList.Num() - 1;
	}
}

void AInteractable::ResetViewDescription()
{
	Data.PrimaryAction.Index = 0;
}
// ENDREGION VIEW DESCRIPTION

// REGION VIEW DESCRIPTION
void AInteractable::EnableSecondaryAction()
{
	Data.SecondaryAction.Active = false;
	OnRep_DataChanged();
}
void AInteractable::DisableSecondaryAction()
{
	Data.SecondaryAction.Active = true;
	OnRep_DataChanged();
}
bool AInteractable::HasSecondaryActionObject()
{
	return (Data.SecondaryAction.ObjectID != "NONE");
}
FName AInteractable::GetSecondaryActionObjectID()
{
	return (Data.SecondaryAction.ObjectID);
}

void AInteractable::RemoveSecondaryActionObject()
{
	Data.SecondaryAction.ObjectID = "NONE";
	OnRep_DataChanged();
}

// ENDREGION VIEW DESCRIPTION



void AInteractable::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractable, Data);

	DOREPLIFETIME(AInteractable, CharacterOverlapping);
}

