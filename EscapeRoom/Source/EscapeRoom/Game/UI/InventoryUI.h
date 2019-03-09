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

	TArray<class UInventorySlot*> Slots;

};
