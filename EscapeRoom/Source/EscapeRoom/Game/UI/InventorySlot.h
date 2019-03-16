// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utils/Definitions.h"
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
	class UImage* Thumbnail;

	UPROPERTY(meta = (BindWidget))
	class UImage* Background;

	UPROPERTY()
	FObjectInteraction ObjectSlot;

	bool bIsEmpty;

public:
		
	FORCEINLINE bool IsEmpty() const { return bIsEmpty; };

	FORCEINLINE FObjectInteraction GetObjectSlot() const { return ObjectSlot; }

	void SetObjectSlot(const FObjectInteraction& Object);

	void SetToDefault();

	void Show();

	void Hide();	

	void Highlight();

	void UnHighlight();

	void SetImageSlot(UTexture2D* Image);
	
};
