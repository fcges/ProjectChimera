// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPGAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) 


UCLASS()
class PROJECTCHIMERA_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	// Constructor
	URPGAttributeSet();

	/** Level Attributes **/
	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_CurrentLevel)
	FGameplayAttributeData CurrentLevel;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, CurrentLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Exp", ReplicatedUsing = OnRep_CurrentExp)
	FGameplayAttributeData CurrentExp;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, CurrentExp)

	UPROPERTY(BlueprintReadOnly, Category = "Exp", ReplicatedUsing = OnRep_MaxExp)
	FGameplayAttributeData MaxExp;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxExp)

	/** Health Attributes **/
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHealth)


	
protected:
	/** Replication Notifies **/
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_CurrentLevel(const FGameplayAttributeData& OldCurrentLevel);

	UFUNCTION()
	virtual void OnRep_CurrentExp(const FGameplayAttributeData& OldCurrentExp);

	UFUNCTION()
	virtual void OnRep_MaxExp(const FGameplayAttributeData& OldMaxExp);

	/** Attribute change handling **/
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
