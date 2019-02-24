// Fill out your copyright notice in the Description page of Project Settings.

#include "ControlWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"


void UControlWidget::SetText(FText Title)
{
	ControlText->SetText(Title);
}

void UControlWidget::Enable()
{
	bIsEnable = true;
}

void UControlWidget::Disable()
{
	bIsEnable = false;
	Hide();
}

void UControlWidget::Show()
{
	if (!bIsEnable) return;

	Control->SetVisibility(ESlateVisibility::Visible);
	ControlText->SetVisibility(ESlateVisibility::Visible);
}

void UControlWidget::Hide()
{
	Control->SetVisibility(ESlateVisibility::Hidden);
	ControlText->SetVisibility(ESlateVisibility::Hidden);
}

