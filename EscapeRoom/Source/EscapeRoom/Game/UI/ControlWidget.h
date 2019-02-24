// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlWidget.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UControlWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* Control;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ControlText;

	bool bIsEnable = true;

public:

	UFUNCTION(BlueprintCallable)
	void SetText(FText Title);

	UFUNCTION(BlueprintCallable)
	void Enable();
	
	UFUNCTION(BlueprintCallable)
	void Disable();

	UFUNCTION(BlueprintCallable)
	void Show();

	UFUNCTION(BlueprintCallable)
	void Hide();

};
