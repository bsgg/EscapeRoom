// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomGameMode.h"
#include "Lobby/LobbyPlayerController.h"
#include "EscapeRoomPlayerState.h"
#include "Utils/Definitions.h"
#include "Game/GameLogic/RoomPlayerStart.h"
#include "Game/EscapeRoomGameState.h"
#include "Game/GameLogic/InteractiveBase.h"

#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"

ARoomGameMode::ARoomGameMode() {}

void ARoomGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetRespawnPoints();

	GetInteractivesInRoom();
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


void ARoomGameMode::GetInteractivesInRoom()
{
	TArray<AActor*> Objects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractiveBase::StaticClass(), Objects);
	for (int32 i = 0; i < Objects.Num(); i++)
	{
		AInteractiveBase* Interactive = Cast<AInteractiveBase>(Objects[i]);
		if (Interactive != nullptr)
		{ 

			UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::GetInteractivesInRoom] Interactive: %s "), *Interactive->GetName());

			InteractiveList.Add(Interactive);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::GetInteractablesInRoom] InteractableList Num: %i "), InteractiveList.Num());
}

AInteractiveBase* ARoomGameMode::FindInteractiveById(FName ID) const
{
	for (int32 i = 0; i < InteractiveList.Num(); i++)
	{
		if (InteractiveList[i]->GetInteractiveID() == ID)
		{
			return InteractiveList[i];
		}
	}

	return nullptr;
}


FObjectInteraction* ARoomGameMode::GetObjectByID(FName ID) const
{
	if (ObjectDB == nullptr) return nullptr;

	return (ObjectDB->FindRow<FObjectInteraction>(ID, TEXT("Object"), true));
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

