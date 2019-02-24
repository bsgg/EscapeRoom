 // Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/MainCharacter.h"
#include "RoomGameMode.h"
#include "Game/Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEscapeZone::AEscapeZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AEscapeZone::HandleOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);	

}

// Called when the game starts or when spawned
void AEscapeZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEscapeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEscapeZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("AEscapeZone::HandleOverlap"));

	AMainCharacter * MainActor = Cast<AMainCharacter>(OtherActor);
	if ((MainActor == nullptr) || (MainActor->GetController() == nullptr)) return;

	//AGamePlayerController* GamePC = Cast<AGamePlayerController>(MActor->GetController());
	//if (GamePC == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Found GamePC"));	

	//if (MActor->bIsCarryingObjective)
	if ((MainActor->GetInventory() != nullptr) && (MainActor->GetInventory()->CheckIfObjectExists(ObjectID)))
	{
		if (OtherActor->Role == ROLE_Authority)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Server Actor: Has Objective"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Client Actor: Has Objective"));
		}

		// Play sound
		if (CompletedRoomSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(this, CompletedRoomSound);
		}

		UWorld* World = GetWorld();
		if (World == nullptr) return;
		ARoomGameMode* GameMode = Cast<ARoomGameMode>(World->GetAuthGameMode());

		// On the client this will be null, but it will do a request to the server to call the function
		if (GameMode == nullptr) 
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] GameMode Is null"));
			return;
		}

		if (OtherActor->Role == ROLE_Authority)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Server Actor: About to call GameMode->CompletedRoom"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Client Actor: About to call GameMode->CompletedRoom"));
		}

		GameMode->CompletedRoom(MainActor, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] NOT OBJECTIVED FOUND"));
	}
}

