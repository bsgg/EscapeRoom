// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"

#include "LobbyMenu.h"
#include "LobbyGameMode.h"
#include "Game/EscapeRoomPlayerState.h"

#include "Game/RoomGameMode.h"
#include "Game/UI/InGamePlayer.h"
#include "Game/UI/InventoryUI.h"
#include "Game/GameLogic/InteractiveBase.h"


#include "UObject/ConstructorHelpers.h"
#include "UnrealNetwork.h"



ALobbyPlayerController::ALobbyPlayerController()
{

	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyMenuBPClass(TEXT("/Game/Lobby/LobbyMenu_WBP"));

	if (LobbyMenuBPClass.Class != nullptr)
	{
		LobbyMenuClass = LobbyMenuBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/Game/UI/InGamePlayer_WBP"));

	if (InGameMenuBPClass.Class != nullptr)
	{
		InGameUIClass = InGameMenuBPClass.Class;
	}
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerState == nullptr) return;

	AEscapeRoomPlayerState* ESPlayerState = Cast<AEscapeRoomPlayerState>(PlayerState);
	if (ESPlayerState == nullptr) return;

	FString Authority = "";

	if (HasAuthority())
	{
		Authority = " -SERVER- ";
	}
	else
	{
		Authority = " -CLIENT- ";
	}

	FString TypeChar = GetEnumValueAsString<ECharacterType>("ECharacterType", ESPlayerState->SelectedCharacter);

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::BeginPlay] %s = ChararcerSelected %s"), *Authority, *TypeChar);
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}



void ALobbyPlayerController::Client_Initialize_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_Initialize]"));

	// Create menu lobby
	if (LobbyMenuClass == nullptr) return;

	LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyMenuClass);

	if (LobbyMenu == nullptr) return;

	LobbyMenu->Setup();

}

void ALobbyPlayerController::Client_RemoveMenu_Implementation()
{
	if (LobbyMenu == nullptr) return;

	LobbyMenu->Teardown();
}

void ALobbyPlayerController::Server_SelectCharacter_Implementation(ECharacterType NewCharacter)
{
	ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr) return;

	if (PlayerState == nullptr) return;

	AEscapeRoomPlayerState* GamePlayerState = Cast<AEscapeRoomPlayerState>(PlayerState);

	if (GamePlayerState != nullptr)
	{
		FString SelectedChar = GetEnumValueAsString< ECharacterType>("ECharacterType", GamePlayerState->SelectedCharacter);
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Server_SelectCharacter]SelectedChar: %s "), *SelectedChar);

		if (GamePlayerState->SelectedCharacter == ECharacterType::VE_NONE)
		{
			GamePlayerState->SelectedCharacter = NewCharacter;
			GM->ChangeCharacterStatus(GamePlayerState->SelectedCharacter);
		}
		else
		{
			GM->ChangeCharacterStatus(GamePlayerState->SelectedCharacter);

			GamePlayerState->SelectedCharacter = NewCharacter;

			GM->ChangeCharacterStatus(NewCharacter);
		}
	}
}

bool ALobbyPlayerController::Server_SelectCharacter_Validate(ECharacterType NewCharacter)
{
	return true;
}


void ALobbyPlayerController::Client_UpdateAvailableCharacters_Implementation(const TArray<FCharacterType>& InAvalaibleCharacters)
{
	AvalaibleCharacters = InAvalaibleCharacters;

	UE_LOG(LogTemp, Warning, TEXT("[Client_UpdateAvailableCharacters] Num AvalaibleCharacters %i "), AvalaibleCharacters.Num());

	if (LobbyMenu == nullptr) return;

	// Update Available character on client
	for (FCharacterType CharT : AvalaibleCharacters)
	{
		// Disable or Update the character
		if (CharT.bIsAvailable)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Client_UpdateAvailableCharacters] CharT %s = Available "), *CharT.CharacterName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[Client_UpdateAvailableCharacters] CharT %s = Not Available "), *CharT.CharacterName.ToString());
		}

		LobbyMenu->SetActiveCharacterButton(CharT.CharacterType, CharT.bIsAvailable);
	}
}

void ALobbyPlayerController::Client_UpdateReadyButton_Implementation()
{
	if (LobbyMenu == nullptr) return;

	LobbyMenu->UpdateReadyState();
}

void ALobbyPlayerController::Client_InitializeRoom_Implementation(TSubclassOf<APawn> PawnToSpawn)
{   
	// TODO: SETUP IN GAME
	if (this->PlayerState == nullptr) return;

	FString Authority = "";

	if (HasAuthority())
	{
		Authority = " -SERVER- ";
	}
	else
	{
		Authority = " -CLIENT- ";
	}

	AEscapeRoomPlayerState* ESPlayerScape = Cast<AEscapeRoomPlayerState>(this->PlayerState);
	if (ESPlayerScape != nullptr)
	{
		FString TCharacter = GetEnumValueAsString<ECharacterType>("ECharacterType", ESPlayerScape->SelectedCharacter);
		FString Message = Authority + " " + TCharacter;
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_InitializeRoom] %s  - MyCharacter: %s"), *Authority, *TCharacter);

		if (InGameUI != nullptr)
		{
			InGameUI->SetPortrait(ESPlayerScape->SelectedCharacter);
			InGameUI->SetInGameMessage(FText::FromString(Message));
		}
	}
}

void ALobbyPlayerController::Client_CreateInGameUI_Implementation()
{
	// Create UI
	if (InGameUIClass == nullptr) return;
	InGameUI = CreateWidget<UInGamePlayer>(this, InGameUIClass);

	if (InGameUI == nullptr) return;
	InGameUI->AddToViewport();
}


// GAMEPLAY INTERACTIVE INTERFACES
void  ALobbyPlayerController::ShowDebugLog_Implementation(const FString& Text)
{
	if (InGameUI == nullptr) return;

	InGameUI->DebugLog(Text);

}
// GAMEPLAY ROOM IMPLEMENTATION


void ALobbyPlayerController::Client_UpdateInGameMessageUI_Implementation(const FString& Text, bool hideMessages)
{
	// Create UI
	if (InGameUI == nullptr) return;	

	if (hideMessages)
	{
		InGameUI->HideMessages();
	}
	else
	{
		InGameUI->ShowMessages();
		InGameUI->SetInGameMessage(FText::FromString(Text));
	}
}


// Inventory
void ALobbyPlayerController::Client_NavigateInventory_Implementation(EDirectionType Direction)
{
	// Create UI
	if (InGameUI == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_NavigateInventory] "));

	InGameUI->NavigateInventory(Direction);
}

void ALobbyPlayerController::Client_ToggleInventory_Implementation()
{
	if (InGameUI == nullptr) return;

	InGameUI->ToggleInventory();
}


void ALobbyPlayerController::Client_AddObjectToSlot_Implementation(const FObjectInteraction& Object)
{
	if (InGameUI == nullptr) return;

	InGameUI->AddObjectToSlot(Object);
}

void ALobbyPlayerController::Client_RemoveObjectFromSlot_Implementation(const FName& ObjectID)
{
	if (InGameUI == nullptr) return;

	InGameUI->RemoveObjectFromSlot(ObjectID);
}

void ALobbyPlayerController::Client_UpdateControlsUI_Implementation(const AInteractiveBase* Interactive)
{
	if (InGameUI == nullptr) return;

	if (Interactive == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_UpdateControlsUI] Interactive == nullptr"));


		InGameUI->HideMessages();
		//InGameUI->HideInventoryIcon();
		InGameUI->HideInspectIcon();
		InGameUI->HideUseIcon();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_UpdateControlsUI] Interactive != nullptr"));

		//InGameUI->HideMessages();

		//InGameUI->ShowInventoryIcon();
		InGameUI->ShowInspectIcon();

		if (Interactive->GetData().HasUseAction)
		{
			InGameUI->ShowUseIcon();
		}
	}
}


void ALobbyPlayerController::Client_UpdateInGame_Implementation(const AInteractiveBase* Interactive)
{
	if (InGameUI == nullptr) return;

	if (Interactive == nullptr)
	{
		InGameUI->HideControls();


		//InGameUI->HideInventoryIcon();
		//InGameUI->HideInspectIcon();
		//InGameUI->HideUseIcon();
	}
	else
	{
		InGameUI->HideControls();
		InGameUI->ShowMessages();

		InGameUI->ShowControls(true, true, false);

		InGameUI->SetInGameMessage(FText::FromString(Interactive->GetDetailedInfo()));

		GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, this, &ALobbyPlayerController::EndInteraction, 2.0f, false);

	}
}

void ALobbyPlayerController::EndInteraction()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);

	Client_HideInGameMessage();
}

void ALobbyPlayerController::Client_HideInGameMessage_Implementation()
{
	if (InGameUI == nullptr) return;

	InGameUI->HideMessages();
}


void ALobbyPlayerController::Client_OnSelectItemInInventory_Implementation(const FObjectInteraction& SelectedObject)
{
	SelectedObjectID = SelectedObject.ID;

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_OnSelectItemInInventory] SelectedObjectID %s = %s"), *SelectedObjectID.ToString(), *SelectedObject.Name.ToString());



	/*if (InGameUI == nullptr) return;

	if (InGameUI->GetInventory() == nullptr) return;	

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_OnSelectItemInInventory_Implementation]"));

	if (InGameUI->GetInventory()->IsReadyToCombine())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_OnSelectItemInInventory_Implementation] Ready to combine"));

		FName idObjectA = InGameUI->GetInventory()->GetObjectIDToCombineA();
		FName idObjectB = InGameUI->GetInventory()->GetObjectIDToCombineB();

		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_OnSelectItemInInventory_Implementation] idObjectA: %s  - idObjectB: %s  "), *idObjectA.ToString(), *idObjectB.ToString());

		FObjectInteraction* newObject = FindCombinedObject(idObjectA, idObjectB);

		if (newObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::OnSelectItemInInventory] newObject found %s"), *newObject->Name.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::OnSelectItemInInventory] newObject not found"));
		}


		


	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::Client_OnSelectItemInInventory_Implementation] OnSelectItem "));

		InGameUI->GetInventory()->OnSelectItem();
	}*/

	//InGameUI->OnSelectItemInventory();
}

// Inventory

FObjectInteraction* ALobbyPlayerController::FindCombinedObject(FName ObjectID_A, FName ObjectID_B) const
{
	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::FindCombinedObject] ObjectID_A %s - ObjectID_B %s"), *ObjectID_A.ToString(), *ObjectID_B.ToString());

	if ((ObjectCombinationDB == nullptr) || (ObjectDB == nullptr)) return nullptr;

	FName CombinedObjectID;

	TArray<FName> RowNames = ObjectCombinationDB->GetRowNames();
	bool CombinationFound = false;

	UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::FindCombinedObject] RowNamesNum %i "), RowNames.Num());

	for (auto& Name : RowNames) // Iterate throught combinations
	{
		FObjectCombination* Row = ObjectCombinationDB->FindRow<FObjectCombination>(Name, TEXT("Object"));
		if (Row)
		{
			// Check if both ID are contained

			UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::FindCombinedObject] Combination Row found %s - %s  "), *Row->ObjectID_A.ToString(), *Row->ObjectID_B.ToString());

			bool firstComb = (Row->ObjectID_A == ObjectID_A) && (Row->ObjectID_B == ObjectID_B);
			bool inverse = (Row->ObjectID_A == ObjectID_B) && (Row->ObjectID_B == ObjectID_A);

			if (firstComb || inverse)	
			{
				UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::FindCombinedObject] Same %s - %s  "), *ObjectID_A.ToString(), *ObjectID_B.ToString());

				CombinedObjectID = Row->ObjectID_Result;
				CombinationFound = true;
				break;
			}
		}
	}

	if (CombinationFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALobbyPlayerController::FindCombinedObject] CombinationFound %s"), *CombinedObjectID.ToString());

		return (ObjectDB->FindRow<FObjectInteraction>(CombinedObjectID, TEXT("Object"), true));
	}

	return nullptr;
}



void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, AvalaibleCharacters);

}