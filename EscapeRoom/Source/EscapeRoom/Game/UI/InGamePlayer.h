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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* RacoonPortrait;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* CatPortrait;

	UPROPERTY(meta = (BindWidget))
	UWidget* MessagesBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InGameMessages;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* SelectedItemIcon;

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

	void ShowMessages();

	void HideMessages();

	void AddObjectToSlot(FObjectInteraction Object);

	void NavigateInventory(EDirectionType Direction);

	void ToggleInventory();

private:

	TArray<class UInventorySlot*> Slots;

	int numberObjectsInventory = 0;

	int CurrentSlotIndex = -1;

	bool bInventoryVisible = false;
};
