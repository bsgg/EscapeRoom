// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractiveInterface.h"
#include "Utils/Definitions.h"

#include "UIBasicInteractive.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UUIBasicInteractive : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetInteraface(IUIBasicInteractiveInterface* UIBasicInteractiveInterface);

	virtual void InitializeWidget(const FName& Combination);

	virtual void OnShowWidget();

public:

	virtual void Navigate(EDirectionType Direction);

	virtual void Interact();


protected:
	virtual bool Initialize() override;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	FText InitialMessages;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	TArray<FText> CorrectAnswerMessages;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive Widget Settings")
	TArray<FText> IncorrectAnswerMessages;

	IUIBasicInteractiveInterface* UIInterface;

	UPROPERTY(meta = (BindWidget))
	class UButton* EnterCombinationButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageText;

	UPROPERTY(meta = (BindWidget))
	class UCombinationUI* Combination_0;

	UPROPERTY(meta = (BindWidget))
	class UCombinationUI* Combination_1;

	UPROPERTY(meta = (BindWidget))
	class UCombinationUI* Combination_2;

	UPROPERTY(meta = (BindWidget))
	class UCombinationUI* Combination_3;

	UFUNCTION()
	void OnExitPressed();

	UFUNCTION()
	void OnEnterCombinationPressed();

private:

	FString Combination;

	bool bIsLocked = false;

	FTimerHandle CompleteWidgetTimerHandler;

	void CompleteWidget();
	
};
