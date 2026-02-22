// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

DECLARE_DELEGATE(FOnResumeRequested);
/**
 * 
 */
UCLASS()
class PROJECTCHIMERA_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnResumeRequested OnResumeRequested;
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ResumeButton;

	UFUNCTION()
	void OnResumeClicked();
};
