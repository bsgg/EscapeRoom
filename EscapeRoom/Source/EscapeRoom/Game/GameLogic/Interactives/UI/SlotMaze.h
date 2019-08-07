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
	void SetSlotColor(FLinearColor Color);



protected:

	UPROPERTY(BlueprintReadonly, VisibleAnywhere, meta = (BindWidget))
	UImage* ImageSlot;
	
};
