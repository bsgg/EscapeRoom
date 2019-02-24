// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/MenuWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);


protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	// REGION MAIN MENU
	UPROPERTY(meta = (BindWidget))
	class UWidget* CreateGameMenuWidget;

	UPROPERTY(meta = (BindWidget))
	class UButton* NewGameMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitMainMenuButton;
	// ENDREGION MAIN MENU

	// REGION MAIN MENU
	UPROPERTY(meta = (BindWidget))
	class UWidget* MessageBlockerWidget;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageBlockerText;	
	FTimerHandle MessageBlockerTimerHandle;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinSessionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSessionButton;
	// ENDREGION MAIN MENU


	// REGION SESSION MENU
	UPROPERTY(meta = (BindWidget))
	class UWidget* SessionListMenuWidget;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ScrollSessionList;

	TSubclassOf<class USessionRow> SessionRowClass;
	TOptional<uint32> SelectedScrollIndex;

	void UpdateSessionList();
	// ENDREGION SESSION MENU

	UFUNCTION()
	void OnNewGamePressed();
	
	UFUNCTION()
	void OnJoinGamePressed();
	
	UFUNCTION()
	void OnQuitPressed();

	UFUNCTION()
	void OnCancelJoinSession();

	UFUNCTION()
	void OnJoinSession();

public:
	void ShowMessageBlocker(FText Message, float HideAfterDelay = 0.0f);
	void HideMessageBlocker();

	void InitializeSessionsList(TArray<FServerData> Data);
	void SelectIndexSessionList(uint32 Index);
	
};
