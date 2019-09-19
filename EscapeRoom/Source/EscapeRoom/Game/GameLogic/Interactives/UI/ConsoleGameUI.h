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

	int maze[5][5] = 
	{
		{0, 1, 2, 1, 0} ,   /*  initializers for row indexed by 0 */
		{1, 0, 3, 0, 3} ,   /*  initializers for row indexed by 1 */
		{0, 2, 0, 4, 1} ,   /*  initializers for row indexed by 2 */
		{1, 1, 1, 2, 1}	    /*  initializers for row indexed by 3 */

	};	

protected:

	UPROPERTY(BlueprintReadonly, EditDefaultsOnly)
	bool IsPlayable;

	UPROPERTY(BlueprintReadonly, VisibleAnywhere, meta = (BindWidget))
	UUniformGridPanel* MazeGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings") 
	int32 GridRows = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	int32 GridColumns = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	FLinearColor DefaultColor;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	FLinearColor HighlightColor;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	FLinearColor UnHighlightColor;

	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	TArray<FLinearColor> SlotColors;


	UPROPERTY(EditDefaultsOnly, Category = "Maze Settings")
	TSubclassOf<USlotMaze> SlotMazeClass;

	TArray<class USlotMaze*> SlotMazeList;

public:

	virtual void InitializeWidget(const FName& Combination) override;

	virtual void OnShowWidget() override;

	virtual void Navigate(EDirectionType Direction) override;

	virtual void OnFaceButtonPress(EFaceButtonType Button) override;


private:

	int32 selectedRowButton = 0;
	int32 selectedColumnButton = 0;
	int32 selectedIndex = -1;
};
