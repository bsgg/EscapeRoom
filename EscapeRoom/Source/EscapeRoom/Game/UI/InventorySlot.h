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
	class UImage* Thumbnail;

	bool bIsEmpty = true;

public:
	void SetImageSlot(UTexture2D* Image);

	FORCEINLINE bool IsEmpty() const { return bIsEmpty;}

	
};
