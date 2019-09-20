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

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	TArray<class USoundBase*> Sounds;


	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	FLinearColor HighlightColor;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	FLinearColor UnHighlightColor;


	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	bool bRandomSequence = false;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitStartSimon = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitButtonToDefaultColor = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitButtonNextColor = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitSwitchTurn = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Simon Logic Settings")
	float WaitGameCompleted = 3.0f;

	// Slots
	UPROPERTY(meta = (BindWidget))
	class USlotMaze* Slot_0;

	UPROPERTY(meta = (BindWidget))
	class USlotMaze* Slot_1;

	UPROPERTY(meta = (BindWidget))
	class USlotMaze* Slot_2;

	UPROPERTY(meta = (BindWidget))
	class USlotMaze* Slot_3;

	UPROPERTY(meta = (BindWidget))
	class USlotMaze* Slot_4;

	TArray<class USlotMaze*> Slots;


protected:
	virtual bool Initialize() override;	

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

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

public:

	virtual void OnShowWidget() override;

	virtual void Navigate(EDirectionType Direction) override;

	virtual void OnFaceButtonPress(EFaceButtonType Button) override;


private:

	void HandleNextPhase();

	void SetSlotToColor(int indexButton, FLinearColor Color);	

	void ResetSlots();
	
};
