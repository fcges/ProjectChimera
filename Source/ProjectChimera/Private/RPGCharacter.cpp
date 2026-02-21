// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "RPGAttributeSet.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));


}

UAbilitySystemComponent* ARPGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeAttributes();

	if (AbilitySystemComponent && AttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute()).AddUObject(this, &ARPGCharacter::HandleHealthChanged);
	}
	
}

void ARPGCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && AttributeSet)
	{
		
	}
}

void ARPGCharacter::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	float NewHealth = Data.NewValue;
	float OldHealth = Data.OldValue;

	float DeltaValue = NewHealth - OldHealth;

	OnHealthChanged(DeltaValue, FGameplayTagContainer());
}

void ARPGCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGCharacter, AbilitySystemComponent);
	DOREPLIFETIME(ARPGCharacter, AttributeSet);
}


