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

	UPROPERTY(meta = (BindWidget))
	class UInventoryUI* Inventory;

	// Inventory Slot
	UPROPERTY(meta = (BindWidget))
	class UInventorySlot* SelectedObject;

	class ALobbyPlayerController* PlayerController;

public:
	void SetPortrait(ECharacterType Character);

	void SetInGameMessage(FText Message);

	void ShowMessages();

	void HideMessages();


	void ShowInventory(const TArray<FObjectInteraction>& Objects);	

	void HideInventory();

	void NavigateInventory(EDirectionType Direction);


	void ShowSelectedObject(const FObjectInteraction& Object);

	void HideSelectedObject();

	void AddObjectToInventory(const FObjectInteraction& Object);

	void UpdateInventory(const TArray<FObjectInteraction>& Objects);

};
