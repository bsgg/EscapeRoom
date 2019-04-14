// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/MenuWidget.h"
#include "Utils/Definitions.h"

#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API ULobbyMenu : public UMenuWidget
{
	GENERATED_BODY()

public:

	void SetActiveCharacterButton(ECharacterType CharacterType, bool Enable);

	void UpdateReadyState();

public:
	void ShowMessageBlocker(FText Message);
	void HideMessageBlocker();

private:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnCatCharacterClicked();

	UFUNCTION()
	void OnRacoonCharacterClicked();

	UFUNCTION()
	void OnReadyButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	void OnCharacterClicked(ECharacterType CharacterType);

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* CatCharacterButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RacoonCharacterButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ReadyButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DebugText;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MessageBlockerWidget;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageBlockerText;

	class ALobbyPlayerController* LobbyPlayerController;	
};
