// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "Utils/MenuInterface.h"
#include "Utils/Definitions.h"
#include "EscapeRoomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UEscapeRoomGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:

	UEscapeRoomGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION()
	void Host(FString ServerName) override;

	UFUNCTION()
	void JoinSession(uint32 Index) override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu() override;

	void StartSession();

	UFUNCTION()
	void OpenSessionListMenu() override;
	void AddPersistentPlayerState(class AEscapeRoomPlayerState* RoomPlayerState);

	void LogPlayerState();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Server Settings")
	TArray<FCharacterType> AvalaibleCharacters;

	TArray<class AEscapeRoomPlayerState*> PersistentPlayerState;

	TSubclassOf<class UUserWidget> MenuClass;
	class UMainMenu* MainMenu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
	
};
