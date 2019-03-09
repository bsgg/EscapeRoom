// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
private:
	virtual bool Initialize() override;

protected:

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* SlotBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidget))
	class UImage* Thumbnail;

	bool bIsEmpty = true;

	UPROPERTY()
	FObjectInteraction ObjectSlot;

public:
	void Show();

	void Hide();

	void SetObjectSlot(const FObjectInteraction& Object);

	void SetTitleSlot(FText Message);

	void SetImageSlot(UTexture2D* Image);

	FORCEINLINE bool IsEmpty() const { return bIsEmpty;}	
};
