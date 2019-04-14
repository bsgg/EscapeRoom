// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombinationUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UCombinationUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* UpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DownButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterText;

	TArray<FString> ABCArray;

	int32 ChainIndex = 0;

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnUpButtonPressed();

	UFUNCTION()
	void OnDownButtonPressed();

public:

	void ResetCombination();

	FString GetCharacter() const;	
	
};
