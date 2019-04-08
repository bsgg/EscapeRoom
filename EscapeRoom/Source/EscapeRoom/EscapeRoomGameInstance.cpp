// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoomGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/UI/MainMenu.h"
#include "Game/EscapeRoomPlayerState.h"

const static FName SESSION_NAME = TEXT("EscapeRoomSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UEscapeRoomGameInstance::UEscapeRoomGameInstance(const FObjectInitializer & ObjectInitializer)
{
	// Find Menus
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MainMenu/UI/MainMenu_WBP")); 

	if (MenuBPClass.Class != nullptr)
	{
		MenuClass = MenuBPClass.Class;
	}
}


void UEscapeRoomGameInstance::Init()
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();

	if (SubSystem != nullptr)
	{
		SessionInterface = SubSystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			//UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::Init] SessionInterface.IsValid"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UEscapeRoomGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UEscapeRoomGameInstance::OnDestroySessionComplete);
		    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UEscapeRoomGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UEscapeRoomGameInstance::OnJoinSessionsComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::Init] Found NO SUBSYSTEM"));
	}
}


void UEscapeRoomGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::CreateSession] Creating %s"), *SESSION_NAME.ToString());

	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		// Switch between bIsLANMatch when using NULL subsystem
		if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}

		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}


void UEscapeRoomGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	// It will not be success if there are more than one session with the same name already created
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnCreateSessionComplete] UNSUCESS"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnCreateSessionComplete] SUCESS SessionName: %s"), *SessionName.ToString());

	// Teardown Menu and change levels
	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}	

	// Check if editor or not
//#if WITH_EDITOR	
	//UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::OnCreateSessionComplete] SESSION CREATED, NOT ABLE TO TRAVEL TO LOBBY ON EDITOR"));
//#else

	UEngine* Engine = GetEngine();

	if (Engine == nullptr) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("[UNetTileMazeGameInstance::Host]"));

	UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::OnCreateSessionComplete] HOST TRAVEL TO LOBBY"));

	UWorld* World = GetWorld();

	if (World == nullptr) return;

	World->ServerTravel("/Game/Maps/Lobby?listen");
//#endif
}

void UEscapeRoomGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		if (MainMenu != nullptr)
		{
			MainMenu->HideMessageBlocker();
		}
		UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnDestroySessionComplete] Sucess "));
		CreateSession();
	}
	else
	{
		if (MainMenu != nullptr)
		{
			MainMenu->ShowMessageBlocker(FText::FromString("Unable to Create Game"), 1.0f);
		}

		UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnDestroySessionComplete] NO Sucess "));
	}
}

void UEscapeRoomGameInstance::Host(FString ServerName)
{
	UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::Host] Start"));

	DesiredServerName = ServerName;

	// Create session if we have a pointer
	if (SessionInterface.IsValid())
	{
		// Check for the session
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::Host] There is an existing session about to remove the current one"));

			if (MainMenu != nullptr)
			{
				MainMenu->ShowMessageBlocker(FText::FromString("Wait. Creating a new Game..."));
			}

			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::Host] About to create session"));

			if (MainMenu != nullptr)
			{
				MainMenu->HideMessageBlocker();
			}

			CreateSession();
		}
	}
	else
	{
		if (MainMenu != nullptr)
		{
			MainMenu->ShowMessageBlocker(FText::FromString("Unable to create game"), 0.5f);
		}

		UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::Host] SessionInterface invalid"));
	}
}


void UEscapeRoomGameInstance::JoinSession(uint32 Index)
{
	if (!SessionInterface.IsValid() || (!SessionSearch.IsValid())) return;

	if (MainMenu != nullptr)
	{
		MainMenu->ShowMessageBlocker(FText::FromString("Joining Session..."));
	}

	if (Index < (uint32)(SessionSearch->SearchResults.Num()))
	{
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	}
}

void UEscapeRoomGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnJoinSessionsComplete]"));

	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}

	if (!SessionInterface.IsValid()) return;

	FString Url;
	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, Url))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnJoinSessionsComplete] Couldn't get Connect String"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (Engine == nullptr) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("[UNetTileMazeGameInstance::Join] Url: %s"), *Url));
	UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnJoinSessionsComplete] Url: %s"), *Url);

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnJoinSessionsComplete] Player Controller DOESN'T EXIST"));
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("[UNetTileMazeGameInstance::Join] PlayerController Found"));

	PlayerController->ClientTravel(Url, ETravelType::TRAVEL_Absolute);
}

void UEscapeRoomGameInstance::StartSession()
{	
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UEscapeRoomGameInstance::EndSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->EndSession(SESSION_NAME);
	}
}

void UEscapeRoomGameInstance::LoadMainMenu()
{
	if (MenuClass == nullptr) return;

	MainMenu = CreateWidget<UMainMenu>(this, MenuClass);
	if (MainMenu == nullptr) return;

	MainMenu->Setup();
	MainMenu->SetMenuInteraface(this);
}

void UEscapeRoomGameInstance::OpenSessionListMenu()
{
	if (MainMenu != nullptr)
	{
		MainMenu->ShowMessageBlocker(FText::FromString(" Looking for Sessions to Join..."));
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::RefreshServerList] Find Sessions"));
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}


void UEscapeRoomGameInstance::OnFindSessionsComplete(bool Success)
{
	if (MainMenu == nullptr) return;

	if (Success && SessionSearch.IsValid())
	{
		if (SessionSearch->SearchResults.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnFindSessionsComplete] No Sessions Find"));
			MainMenu->ShowMessageBlocker(FText::FromString("No Sessions Found"), 1.0f);
		}
		else
		{
			TArray<FServerData> ServerData;
			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnFindSessionsComplete] Session Name %s"), *SearchResult.GetSessionIdStr());

				FServerData Data;
				FString ServerName;
				if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
				{
					UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnFindSessionsComplete] Data found in settings %s"), *ServerName);
					Data.Name = ServerName;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnFindSessionsComplete] Data NOT found in settings"));

					Data.Name = "Could not find name";
				}

				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
				Data.HostUsername = SearchResult.Session.OwningUserName;

				ServerData.Add(Data);
			}

			MainMenu->HideMessageBlocker();
			MainMenu->InitializeSessionsList(ServerData);
		}
	}
	else
	{
		MainMenu->ShowMessageBlocker(FText::FromString("Error No Sessions Found"), 1.0f);
	}
}
void UEscapeRoomGameInstance::LogPlayerState()
{
	UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::PlayerState] NPersistentPlayerState %i"), PersistentPlayerState.Num());

	for (AEscapeRoomPlayerState* RoomPS : PersistentPlayerState)
	{
		FString TypeChar = GetEnumValueAsString<ECharacterType>("ECharacterType", RoomPS->SelectedCharacter);

		UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::PlayerState]TypeChar %s"),*TypeChar);
	}
}

void UEscapeRoomGameInstance::AddPersistentPlayerState(class AEscapeRoomPlayerState* RoomPlayerState)
{
	PersistentPlayerState.Add(RoomPlayerState);
}