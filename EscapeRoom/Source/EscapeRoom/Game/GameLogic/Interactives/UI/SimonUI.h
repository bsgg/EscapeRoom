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
	FLinearColor DefaultButtonColor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	TArray<FLinearColor> ButtonColors;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	bool bRandomSequence = false;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitStartSimon = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitButtonToDefaultColor = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitButtonNextColor = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitSwitchTurn = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitGameCompleted = 2.0f;

	

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

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UFUNCTION()
	void OnColorButton_1Pressed();

	UFUNCTION()
	void OnColorButton_2Pressed();

	UFUNCTION()
	void OnColorButton_3Pressed();

	UFUNCTION()
	void OnColorButton_4Pressed();

	
	TArray<class UButton*> ColorButtons;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	TArray<int> ColorSequence;

	int IndexSequence;

	int IndexEndSequence;

	
	bool bLockInput;

	bool bPlayerTurn = false;	

	int ButtonPressedIndex = -1;


	int NextGamePhase;

	bool bWait;

	float ElpasedWait = 0.0f;

	float WaitTime;

private:
	void SetButtonToColor(int indexButton, FLinearColor Color);

	void OnButtonPressed(int indexButton);

	void HandleNextPhase();

public:

	virtual void InitializeWidget(const FName& Combination) override;

	virtual void OnShowWidget() override;
	
};
