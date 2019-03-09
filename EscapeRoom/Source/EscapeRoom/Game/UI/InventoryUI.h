// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual bool Initialize() override;

public:
	void Show(const TArray<FObjectInteraction>& Objects);
	void Hide();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DescriptionSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* InventoryGrid;


	// Slots
	TArray<class UInventorySlot*> Slots;

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

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_5;

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_6;

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_7;

	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* Slot_8;


};
