// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicInteractive.h"
#include "Characters/MainCharacter.h"
#include "Lobby/LobbyPlayerController.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"
#include "Game/RoomGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"

// Sets default values
ABasicInteractive::ABasicInteractive()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));  
	RootComponent = RootScene;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Mesh->SetupAttachment(RootComponent);


	ConstructorHelpers::FObjectFinder<UStaticMesh> highlightAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> highlightMaterial(TEXT("/Game/Common/Materials/HighlightMaterial/M_Highlight"));

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
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABasicInteractive::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABasicInteractive::EndOverlap);

	SetReplicates(true);
}


void ABasicInteractive::BeginPlay()
{
	Super::BeginPlay();

	HighlightMesh->SetVisibility(false, true);
}

void ABasicInteractive::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
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

			Properties.IndexInspectMessage = 0;

		}
	}
}

void ABasicInteractive::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

				Properties.IndexInspectMessage = 0;
			}
		}
	}
}

void ABasicInteractive::PlayInteractSound()
{
	if (InteractSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, InteractSound);
	}

}


/////// IInteractiveInterface IMPLEMENTATION /////////
void ABasicInteractive::Inspect(APawn* PawnInstigator)
{
	auto PlayerController = Cast<ALobbyPlayerController>(PawnInstigator->GetController());

	if (PlayerController)
	{
		PlayerController->ShowDebugLog("Inspecting");

		FString Text = "";

		if (Properties.EnableDefaultInspectMessage)
		{
			Text = Properties.DefaultInspectMessage.ToString();

			PlayerController->ShowMessage(Text, 2.0f);

			if (CharacterOverlapping != nullptr)
			{
				CharacterOverlapping->StartGesture(EGestureType::VE_INSPECT, 2.0f);
			}
		}
		else if ((Properties.InspectMessages.Num() > 0) && (Properties.IndexInspectMessage < Properties.InspectMessages.Num()))
		{
			Text = Properties.InspectMessages[Properties.IndexInspectMessage].ToString();

			PlayerController->ShowMessage(Text, 2.0f);

			if (CharacterOverlapping != nullptr)
			{
				CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
			}

			Properties.IndexInspectMessage += 1;

			if (Properties.IndexInspectMessage >= Properties.InspectMessages.Num())
			{
				Properties.IndexInspectMessage = Properties.InspectMessages.Num() - 1;
			}
		}
	}
}

void ABasicInteractive::StartInteract(APawn* PawnInstigator)
{
	CurrentController = Cast<ALobbyPlayerController>(PawnInstigator->GetController());

	if (CurrentController)
	{
		CurrentController->ShowDebugLog("Interacting");
	}
}

FString ABasicInteractive::GetInteractID()
{
	return Properties.ID.ToString();
}

/////// IInteractiveInterface IMPLEMENTATION /////////


/////// IUIBasicInteractiveInterface IMPLEMENTATION /////////
bool ABasicInteractive::HasWidget()
{
	if (WidgetID != "NONE") return true;

	return false;
}

void ABasicInteractive::ShowWidget()
{
	if (CurrentController == nullptr) return;

	CurrentController->CreateInteractiveUI(WidgetID);

	if (CurrentController->GetInteractiveUI() != nullptr)
	{
		CurrentController->GetInteractiveUI()->SetInteraface(this);
	}
}

void ABasicInteractive::ExitUI()
{
	if (CurrentController == nullptr) return;

	CurrentController->RemoveInteractiveUI();
}

void ABasicInteractive::OnComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("[ABasicInteractive::OnComplete] Called"));

	if (CurrentController == nullptr) return;

	CurrentController->RemoveInteractiveUI();

}

/////// Connected Interactive /////////

ABasicInteractive* ABasicInteractive::FindConnectedInteractive()
{
	if (ConnectedInteractiveID == "NONE") return nullptr;

	ARoomGameMode* GM = Cast<ARoomGameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr) return  nullptr;

	return GM->FindInteractiveById(ConnectedInteractiveID);
}

void ABasicInteractive::InteractOnConnectedInteractive() {}

/////// Connected Interactive /////////


/////// IUIBasicInteractiveInterface IMPLEMENTATION /////////


void ABasicInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicInteractive, ConnectedInteractiveID);

	DOREPLIFETIME(ABasicInteractive, Properties);
}



