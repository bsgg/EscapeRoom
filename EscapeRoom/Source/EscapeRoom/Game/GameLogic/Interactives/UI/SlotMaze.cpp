// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotMaze.h"

void USlotMaze::SetSlotColor(FLinearColor Color)
{
	if (ImageSlot == nullptr) return;

	ImageSlot->SetColorAndOpacity(Color);
}