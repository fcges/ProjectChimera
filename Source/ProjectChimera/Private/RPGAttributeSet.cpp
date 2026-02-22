// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "CombatCharacter.h"

URPGAttributeSet::URPGAttributeSet()
{
	//Set default values
	CurrentLevel.SetBaseValue(1.f);
	CurrentLevel.SetCurrentValue(1.f);

	CurrentExp.SetBaseValue(0.f);
	CurrentExp.SetCurrentValue(0.f);

	MaxExp.SetBaseValue(100.f);
	MaxExp.SetCurrentValue(100.f);
	
	CurrentHealth.SetBaseValue(100.f);
	CurrentHealth.SetCurrentValue(100.f);

	MaxHealth.SetBaseValue(100.f);
	MaxHealth.SetCurrentValue(100.f);
}

void URPGAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CurrentHealth, OldCurrentHealth);
}

void URPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth, OldMaxHealth);
}

void URPGAttributeSet::OnRep_CurrentLevel(const FGameplayAttributeData& OldCurrentLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CurrentLevel, OldCurrentLevel);
}

void URPGAttributeSet::OnRep_CurrentExp(const FGameplayAttributeData& OldCurrentExp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CurrentExp, OldCurrentExp);
}

void URPGAttributeSet::OnRep_MaxExp(const FGameplayAttributeData& OldMaxExp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxExp, OldMaxExp);
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetCurrentHealthAttribute())
	{
		const float MaxHealthValue = MaxHealth.GetCurrentValue();
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxHealthValue);
	}
	else if (Attribute == GetCurrentExpAttribute())
	{
		const float MaxExpValue = MaxExp.GetCurrentValue();
		while (NewValue >= MaxExpValue) {
			NewValue -= MaxExpValue;
			CurrentLevel.SetCurrentValue(CurrentLevel.GetCurrentValue() + 1);
			ApplyStatEffect();
		}
	}
}

void URPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		float NewHealth = FMath::Clamp(CurrentHealth.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue());
		SetCurrentHealth(NewHealth);
	}
	else if (Data.EvaluatedData.Attribute == GetCurrentExpAttribute())
	{
		const float MaxExpValue = MaxExp.GetCurrentValue();
		float NewExp = CurrentExp.GetCurrentValue();
		while (NewExp >= MaxExpValue) {
			NewExp -= MaxExpValue;
			CurrentLevel.SetCurrentValue(CurrentLevel.GetCurrentValue() + 1);
			ApplyStatEffect();
		}
		SetCurrentExp(NewExp);
	}
}

void URPGAttributeSet::ApplyStatEffect()
{
	IAbilitySystemInterface* GasInterface = Cast<IAbilitySystemInterface>(GetOwningActor());
	if (GasInterface)
	{
		UAbilitySystemComponent* ASC = GasInterface->GetAbilitySystemComponent();
		if (IsValid(ASC))
		{
			ACombatCharacter* OwnerCharacter = Cast<ACombatCharacter>(GetOwningActor());
			if (OwnerCharacter && OwnerCharacter->StatEffect)
			{
				FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(
					OwnerCharacter->StatEffect, CurrentLevel.GetCurrentValue(), Context
				);
				if (Spec.IsValid())
				{
					ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
				}
			}
		}
	}
}

// Defines which properties are replicated over the network
void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate CurrentHealth and MaxHealth to all clients
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}