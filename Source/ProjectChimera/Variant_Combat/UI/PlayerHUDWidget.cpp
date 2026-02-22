// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

void UPlayerHUDWidget::UpdateHealthBar(float Current, float Max)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Max > 0 ? Current / Max : 0.f);
	}
	if (HealthText)
	{
		HealthText->SetText(
		FText::FromString(FString::Printf(TEXT("%d / %d"), (int)Current, (int)Max)));
	}
}

void UPlayerHUDWidget::UpdateExp(float Current, float Max)
{
	if (ExpBar)
	{
		ExpBar->SetPercent(Max > 0 ? Current / Max : 0.f);
		ExpLabel->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), (int)Current, (int)Max)));
	}
}

void UPlayerHUDWidget::UpdateLevel(int32 NewLevel)
{
	if (LevelText)
	{
		LevelText->SetText(FText::FromString(FString::Printf(TEXT("Lv. %d"), NewLevel)));
	}
}