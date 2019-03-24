// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set pickup mesh as a root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	RootComponent = RootScene;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	OtherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OtherMesh"));
	OtherMesh->SetupAttachment(RootComponent);	

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APickup::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &APickup::EndOverlap);

	// Sync this actor on multiplayer
	SetReplicates(true);

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	if (OtherMesh != nullptr)
	{
		OtherMesh->SetVisibility(bIsActive);
	}	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::OnRep_IsActive()
{
	// Only on the client
	ShowOtherMesh();
}

void APickup::ShowOtherMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("[APickup::UpdateTestState]"));
	// SHOW UI??
	if (OtherMesh != nullptr)
	{
		OtherMesh->SetVisibility(bIsActive);
	}

	if (OverlappedSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, OverlappedSound);
	}
}


void APickup::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[APickup::BeginOverlap]"));

	if (!bIsActive) return;

	if (OtherActor == nullptr) return;

	// Play OverlappedSound everywhere?
	//if (OverlappedSound != nullptr)
	//{
		//UGameplayStatics::PlaySound2D(this, OverlappedSound);
	//}

	// PlayEffect() This can be on local client	

	// Destroy pickup on the server
	if (Role == ROLE_Authority)
	{
		AMainCharacter* Char = Cast<AMainCharacter>(OtherActor);
		//if (Char == nullptr) return;
		//Char->bIsCarryingObjective = true;

		//Char->OnOverlapPickup(this, ObjectID);

		// Because this is replicated, and changed, this will be called on the server and all the clients will be notify
		bIsActive = false;

		// Call this function here, this way we avoid having difference between server and client
		// OnRep_IsActive is not called on the server
		// If we have a client-host and normal client, OnRep_IsActive only will be executed on the client, that's why we need to called it here as well.
		OnRep_IsActive();

		//Destroy();
	}
	
}

void APickup::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsActive) return;

	if (OtherActor == nullptr) return;

	if (Role == ROLE_Authority)
	{
		//AMainCharacter* Char = Cast<AMainCharacter>(OtherActor);
		//if (Char == nullptr) return;

		// Empty pickup
		//Char->OnOverlapPickup(nullptr, FName(TEXT("NONE")));

		bIsActive = true;

		OnRep_IsActive();
	}
}



void APickup::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickup, bIsActive);

}


