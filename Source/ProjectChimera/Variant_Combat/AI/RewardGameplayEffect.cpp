// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_Combat/AI/RewardGameplayEffect.h"
#include "Public/RPGAttributeSet.h"

URewardGameplayEffect::URewardGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Processing exp reward
	FGameplayModifierInfo ExpMod;
	ExpMod.Attribute = URPGAttributeSet::GetCurrentExpAttribute();
	ExpMod.ModifierOp = EGameplayModOp::Additive;

	// EXP value to reward player, set in the blueprint.
	FScalableFloat ExpAmount;
	ExpMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(ExpAmount);

	Modifiers.Add(ExpMod);
}