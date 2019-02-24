// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utils/MenuInterface.h"

#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();

	// Method to stop the widget working
	void Teardown();

	void SetMenuInteraface(IMenuInterface* MenuInterface);

protected:
	IMenuInterface* MenuInterface;
	
};
