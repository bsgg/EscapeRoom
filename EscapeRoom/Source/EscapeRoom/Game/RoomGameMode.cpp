// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomGameMode.h"
#include "Lobby/LobbyPlayerController.h"
#include "EscapeRoomPlayerState.h"
#include "Utils/Definitions.h"
#include "Game/GameLogic/RoomPlayerStart.h"
#include "Game/EscapeRoomGameState.h"
#include "Game/GameLogic/Interactable.h"

#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"

ARoomGameMode::ARoomGameMode() {}

void ARoomGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetRespawnPoints();

	GetInteractablesInRoom();
}


// CALLED WHEN PLAYED FROM EDITOR (NOT IN SEAMLESS TRAVEL)
void ARoomGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(NewPlayer);
	if (LobbyPC == nullptr) return;

	++NumTravellers;

	PlayerInGameList.Add(LobbyPC);

	if (PlayerInGameList.Num() < 2) return;

	int indexTraveler = 0;
	for (ALobbyPlayerController* IngamePC : PlayerInGameList)
	{
		IngamePC->Client_CreateInGameUI();

		//IngamePC->SetActorLocation()
		if (indexTraveler < SpawnPoints.Num())
		{
			FVector NewLocation = SpawnPoints[indexTraveler]->GetActorLocation();
			IngamePC->GetPawn()->SetActorLocation(NewLocation);
		}
		indexTraveler++;
	}

}

// CALLED WHEN PLAYED FROM EDITOR (NOT IN SEAMLESS TRAVEL)
void ARoomGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ARoomGameMode::HandleSeamlessTravelPlayer(AController *& PlayerController)
{
	//UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::HandleSeamlessTravelPlayer]"));
	if (PlayerController == nullptr) return;

	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PlayerController);
	if (LobbyPC == nullptr) return;

	++NumTravellers;

	PlayerInGameList.Add(LobbyPC);

	UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::HandleSeamlessTravelPlayer] Traveller %i: Client_InitializeRoom "), PlayerInGameList.Num());
	
	if (PlayerInGameList.Num() < 2) return;
		
	UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::HandleSeamlessTravelPlayer] All Player Spawned! Checking Player States "), PlayerInGameList.Num());
	int indexTraveler = 0;

	for (ALobbyPlayerController* IngamePC : PlayerInGameList)
	{
		if (IngamePC->PlayerState == nullptr) continue;
		
		AEscapeRoomPlayerState* ESPlayerScape = Cast< AEscapeRoomPlayerState>(IngamePC->PlayerState);
		if (ESPlayerScape == nullptr)  continue;

		FString TCharacter = GetEnumValueAsString<ECharacterType>("ECharacterType", ESPlayerScape->SelectedCharacter);
		UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::HandleSeamlessTravelPlayer] Traveller %i = %s About to initialize"), indexTraveler, *TCharacter);

		// Find SpawnPoint 
		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		for (ARoomPlayerStart* SpawnPoint : SpawnPoints)
		{
			if (SpawnPoint->GetCharacterToSpawn() == ESPlayerScape->SelectedCharacter)
			{
				SpawnLocation = SpawnPoint->GetActorLocation();
				SpawnRotation = SpawnPoint->GetActorRotation();
				break;
			}
		}

		for (FCharacterType CharT : AvalaibleCharacters)
		{
			if (CharT.CharacterType == ESPlayerScape->SelectedCharacter)
			{
				FActorSpawnParameters SpawnParamenters;
				SpawnParamenters.Owner = this;
				SpawnParamenters.Instigator = nullptr;

				APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CharT.Pawn, SpawnLocation, SpawnRotation, SpawnParamenters);
				if (SpawnedPawn == nullptr) return;
				UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_SpawnCharacter] Spawned!"));

				IngamePC->Possess(SpawnedPawn);
				IngamePC->Client_CreateInGameUI();
				IngamePC->Client_InitializeRoom(CharT.Pawn);
			}
		}

		indexTraveler++;
		
	}
}

void ARoomGameMode::GetRespawnPoints()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		ARoomPlayerStart* PlayerStart = Cast<ARoomPlayerStart>(PlayerStarts[i]);
		if (PlayerStart != nullptr)
		{
			SpawnPoints.Add(PlayerStart);
		}
	}
}


void ARoomGameMode::GetInteractablesInRoom()
{
	TArray<AActor*> Objects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractable::StaticClass(), Objects);
	for (int32 i = 0; i < Objects.Num(); i++)
	{
		AInteractable* Interactable = Cast<AInteractable>(Objects[i]);
		if (Interactable != nullptr)
		{ 

			UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::GetInteractablesInRoom] Interactable: %s "), *Interactable->GetName());

			InteractableList.Add(Interactable);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::GetInteractablesInRoom] InteractableList Num: %i "), InteractableList.Num());
}


FObjectInteraction* ARoomGameMode::GetObjectByID(FName ID) const
{
	if (ObjectDB == nullptr) return nullptr;

	return (ObjectDB->FindRow<FObjectInteraction>(ID, TEXT("Object"), true));
}

FInteractionData* ARoomGameMode::GetInteractableByID(FName ID) const
{
	if (InteractableDB == nullptr) return nullptr;

	return (InteractableDB->FindRow<FInteractionData>(ID, TEXT("Interactable"), true));	
}

FActionData ARoomGameMode::GetInteractableSecondaryAction(FName ID) const
{
	FActionData Data;
	Data.InteractionType = EInteractionType::VE_NONE;

	if (InteractableDB == nullptr) return Data;

	FInteractionData* Interaction = GetInteractableByID(ID);

	if (Interaction == nullptr) return Data;

	if (Interaction->SecondaryAction.InteractionType != EInteractionType::VE_NONE)
	{
		return Interaction->SecondaryAction;
	}

	return Data;
}


void ARoomGameMode::CompletedRoom(APawn* InstigatorPawn, bool bSuccess)
{
	if (InstigatorPawn == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[AMainGameMode::CompletedRoom] Complete Room"));

	// Called EscapeRoom to change the state of the game and let everyone know the new state
	AEscapeRoomGameState* GameState = GetGameState<AEscapeRoomGameState>();

	if (GameState == nullptr) return;
	GameState->MulticastOnRoomCompleted(InstigatorPawn, bSuccess);	

	OnRoomCompleted(InstigatorPawn, bSuccess);
}

void ARoomGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoomGameMode, SpawnPoints);
}

