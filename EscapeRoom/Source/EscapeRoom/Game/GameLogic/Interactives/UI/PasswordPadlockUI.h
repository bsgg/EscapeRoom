// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"
#include "PasswordPadlockUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UPasswordPadlockUI : public UUIBasicInteractive
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UCombinationUI* Combination_0;

	UPROPERTY(meta = (BindWidget))
	class UCombinationUI* Combination_1;

	UPROPERTY(meta = (BindWidget))
	class UCombinationUI* Combination_2;

	UPROPERTY(meta = (BindWidget))
	class UCombinationUI* Combination_3;


protected:
	virtual bool Initialize() override;

	virtual void CompleteWidget() override;

private:	

	FString Combination;

	bool bIsLocked = false;

	FTimerHandle CompleteWidgetTimerHandler;

	void CheckCombination();

public:

	virtual void InitializeWidget(const FName& InCombination) override;


	
	
};
