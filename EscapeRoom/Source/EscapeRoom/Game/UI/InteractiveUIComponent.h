// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractiveUIComponent.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UInteractiveUIComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UInteractiveUIComponent();

	void Initialize();

	FORCEINLINE class UInteractableUI* GetUI() const { return UI; }

protected:

	class UInteractableUI* UI;

	virtual void BeginPlay() override;
	
};
