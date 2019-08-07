// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"
#include "Components/UniformGridPanel.h"
#include "Game/GameLogic/Interactives/UI/SlotMaze.h"
#include "ConsoleGameUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API UConsoleGameUI : public UUIBasicInteractive
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:

	int maze[4][4] = 
	{
		{0, 1, 1, 1} ,   /*  initializers for row indexed by 0 */
		{1, 0, 0, 0} ,   /*  initializers for row indexed by 1 */
		{1, 1, 0, 0} ,
		{1, 1, 1, 0}	/*  initializers for row indexed by 2 */

	};

	

protected:

	UPROPERTY(BlueprintReadonly, VisibleAnywhere, meta = (BindWidget))
	UUniformGridPanel* MazeGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	int32 GridRows = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	int32 GridColumns = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	FLinearColor DefaultSlotColor;


	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	TSubclassOf<USlotMaze> SlotMazeClass;

public:

	virtual void InitializeWidget(const FName& Combination) override;

	virtual void OnShowWidget() override;


	
};
