// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RewardGameplayEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCHIMERA_API URewardGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "EXP")
	int32 Exp = 10;

public:
	URewardGameplayEffect();

};
