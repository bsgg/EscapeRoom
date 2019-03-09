// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UInteractableUI : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual bool Initialize() override;

public:
	
	FORCEINLINE class UControlWidget* GetAControl() const { return AControl; }
	FORCEINLINE class UControlWidget* GetXControl() const { return XControl; }

protected:

	UPROPERTY(meta = (BindWidget))
	class UControlWidget* AControl;

	UPROPERTY(meta = (BindWidget))
	class UControlWidget* XControl;

};
