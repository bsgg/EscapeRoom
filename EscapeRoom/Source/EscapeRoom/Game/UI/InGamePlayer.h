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

public:

	void SetPortrait(ECharacterType Character);

	void SetInGameMessage(FText Message);

	void ShowControls(bool ShowInventoryIcon, bool ShowInspectIcon, bool ShowUseIcon);
	void HideControls();

	void ShowInventoryIcon();
	void HideInventoryIcon();

	void ShowInspectIcon();
	void HideInspectIcon();

	void ShowUseIcon();
	void HideUseIcon();

	void ShowMessage(const FString& Text);
	void HideMessage();

	void ShowMessages();

	void HideMessages();



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

	

	bool bInventoryVisible = false;
};
