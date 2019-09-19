// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SlotMaze.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API USlotMaze : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetIndex(int32 index)
	{
		Index = index;
	}

	int32 GetIndex() { return Index; }

	void SetSlotColor(FLinearColor Color);

	void SetSlotHighlightColor(FLinearColor Color);

protected:

	UPROPERTY(BlueprintReadonly, VisibleAnywhere, meta = (BindWidget))
	UImage* ImageSlot;
	

	UPROPERTY(BlueprintReadonly, VisibleAnywhere, meta = (BindWidget))
	UImage* ImageSlotHighlight;

private:

	int32 Index;
};
