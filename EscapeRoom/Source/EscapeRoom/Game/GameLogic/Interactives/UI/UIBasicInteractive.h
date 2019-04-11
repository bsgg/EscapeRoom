// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractiveInterface.h"
#include "UIBasicInteractive.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UUIBasicInteractive : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetInteraface(IUIBasicInteractiveInterface* UIInterface);

protected:
	virtual bool Initialize() override;

	IUIBasicInteractiveInterface* UIInterface;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UFUNCTION()
	void OnExitPressed();
	
};
