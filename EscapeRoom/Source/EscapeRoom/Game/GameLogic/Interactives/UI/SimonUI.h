// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"
#include "SimonUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API USimonUI : public UUIBasicInteractive
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	TArray<FLinearColor> ColorSequence;

	UPROPERTY(meta = (BindWidget))
	class UButton* ColorButton_1;

	UPROPERTY(meta = (BindWidget))
	class UButton* ColorButton_2;

	UPROPERTY(meta = (BindWidget))
	class UButton* ColorButton_3;

	UPROPERTY(meta = (BindWidget))
	class UButton* ColorButton_4;

protected:
	virtual bool Initialize() override;

	

	UFUNCTION()
	void OnColorButton_1Pressed();

	UFUNCTION()
	void OnColorButton_2Pressed();

	UFUNCTION()
	void OnColorButton_3Pressed();

	UFUNCTION()
	void OnColorButton_4Pressed();

public:

	virtual void InitializeWidget(const FName& Combination) override;

	virtual void OnShowWidget() override;
	
};
