// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Utils/Definitions.h"
#include "InGamePlayer.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UInGamePlayer : public UUserWidget
{
	GENERATED_BODY()
private:
	virtual bool Initialize() override;

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DebugText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget* RacoonPortraitIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget* CatPortraitIcon;

	UPROPERTY(meta = (BindWidget))
	UWidget* InGameMessagesBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InGameMessageText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget* ControlsBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* SelectedItemIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget* InventoryIconBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget* InspectIconBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget* UseIconBox;

protected:

	// Inventory Grid
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* InventoryGrid;

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_0;

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_1;

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_2;

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_3;

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_4;

	class ALobbyPlayerController* PlayerController;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:	

	void SetPortrait(ECharacterType Character);

	void ShowControls(bool ShowInventoryIcon, bool ShowInspectIcon, bool ShowUseIcon);
	void HideControls();

	void ShowMessage(const FString& Text, float time);
	void HideMessage();

	void NavigateInventory(EDirectionType Direction);

	void ToggleInventory();

	void AddObjectToSlot(FObjectInteraction Object);

	void RemoveObjectFromSlot(const FName& ObjID);

	void DebugLog(const FString& Text);

	FName GetSelectedItem();

private:

	TArray<class UInventorySlot*> Slots;

	int numberObjectsInventory = 0;

	int CurrentSlotIndex = -1;

	FName SelectedObjectID;

	bool bControlsVisible = false;

	bool bInventoryVisible = false;

	bool bWaitingToHideMessage = false;

	FTimerHandle MessageTimerHandle;


private:

	bool bTypewriterMessage = false;

	FString MessageToTypewrite;

	FString CurrentMessage;

	//int32 LettersMessageLeft;

	int32 LetterIndex;

	float CurrentTimeBetweenLetters = 0.0f;

	float DelayBetweenLetters = 0.5f;

	float TotalTimeTypeWritting = 0.0f;


};
