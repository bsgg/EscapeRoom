// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CombinationUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UCombinationUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	//UPROPERTY(meta = (BindWidget))
	//class UButton* UpButton;

	//UPROPERTY(meta = (BindWidget))
	//class UButton* DownButton;

	UPROPERTY(BlueprintReadonly, VisibleAnywhere, meta = (BindWidget))
	UImage* Background;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterText;

	UPROPERTY(EditDefaultsOnly, Category = "Combination UI Settings")
	FLinearColor SelectedColor;

	UPROPERTY(EditDefaultsOnly, Category = "Combination UI Settings")
	FLinearColor UnSelectedColor;

	TArray<FString> ABCArray;

	int32 ChainIndex = 0;

protected:
	virtual bool Initialize() override;

	//UFUNCTION()
	//void OnUpButtonPressed();

	//UFUNCTION()
	//void OnDownButtonPressed();

public:

	void ResetCombination();

	FString GetCharacter() const;	

	bool bIsLocked = false;

	void Locked();

	void Unlocked();

	void OnChangeCharacterPress();

	void OnSelect();

	void OnDeselect();

	
};
