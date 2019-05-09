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

	//UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	

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

	TArray<int> ColorSequence;

	int IndexSequence;

	int IndexEndSequence;

	bool bLockInput;

	bool bWaitToSetColor;

	bool bWaitToSetDefaultColor;

	float ElpasedToSetColor = 0.0f;

	float WaitTimeToSetColor = 0.6f;


	bool bPlayerTurn = false;

	int ButtonPressedIndex = -1;

private:
	void SetButtonToColor(int indexButton, FLinearColor Color);

	void OnButtonPressed(int indexButton);

public:

	virtual void InitializeWidget(const FName& Combination) override;

	virtual void OnShowWidget() override;
	
};
