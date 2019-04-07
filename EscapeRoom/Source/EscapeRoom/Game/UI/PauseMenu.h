// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/MenuWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UPauseMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitGameButton;

	UFUNCTION()
	void OnResumeGamePressed();
	
	UFUNCTION()
	void OnExitGamePressed();
};
