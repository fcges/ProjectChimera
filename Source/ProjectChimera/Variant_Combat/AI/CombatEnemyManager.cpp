// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_Combat/AI/CombatEnemyManager.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemGlobals.h"
#include "RPGAttributeSet.h"
#include "AttributeSet.h"
#include "CombatEnemy.h"
#include "GameFramework/Character.h"
#include <CombatEnemySpawner.h>
//#include <mutex>

// Sets default values
ACombatEnemyManager::ACombatEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	// Initial Setup
	EnemyToSpawnCount = 3;
	EliteEnemyToSpawnCount = 0;

	// Binding delegate function for attribute listening
	IAbilitySystemInterface* GasInterfacePtr = Cast<IAbilitySystemInterface>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (GasInterfacePtr)
	{
		AbilitySystemComponent = GasInterfacePtr->GetAbilitySystemComponent();
		if (IsValid(AbilitySystemComponent))
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URPGAttributeSet::GetCurrentLevelAttribute()).AddUObject(this, &ACombatEnemyManager::OnPlayerLevelUp);
		}
	}

	for (int i = 0; i < EnemySpawner.Num(); i++)
	{
		ACombatEnemySpawner* Spawner = Cast<ACombatEnemySpawner>(EnemySpawner[i]);
		Spawner->RegisterCallback(this);
	}
	for (int i = 0; i < EliteEnemySpawner.Num(); i++)
	{
		ACombatEnemySpawner* EliteSpawner = Cast<ACombatEnemySpawner>(EnemySpawner[i]);
		EliteSpawner->RegisterCallback(this);
	}
		
}

// Called every frame
void ACombatEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatEnemyManager::OnEnemySpawned()
{
	//std::lock_guard<std::mutex> Lock(Mtx);
	EnemyToSpawnCount--;

}

void ACombatEnemyManager::OnEliteEnemySpawned()
{
	//std::lock_guard<std::mutex> Lock(Mtx);
	EliteEnemyToSpawnCount--;
}

bool ACombatEnemyManager::GetMoreEnemyToSpawn(TArray<FName> EnemyTags)
{
	if (EnemyTags.Contains("Enemy"))
	{
		if (EnemyToSpawnCount > 0) {
			return true;
		}
		else if (IsValid(AbilitySystemComponent))
		{
			bool bFound = false;
			int32 Level = AbilitySystemComponent->GetGameplayAttributeValue(URPGAttributeSet::GetCurrentLevelAttribute(), bFound);
			if (bFound)
			{
				EnemyToSpawnCount += Level * 3;
			}
			UE_LOG(LogTemp, Display, TEXT("Enemy replenished. New amount %d"), EnemyToSpawnCount);
			return bFound;
		}
	}
	if (EnemyTags.Contains("EliteEnemy") && EliteEnemyToSpawnCount > 0) {
		return true;
	}
	return false;
}

void ACombatEnemyManager::OnPlayerLevelUp(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("Player level up. New level data %d"), (int32)Data.NewValue);
	EnemyToSpawnCount += 3 * Data.NewValue;
	if ((int32)Data.NewValue % 3 == 0)
	{
		EliteEnemyToSpawnCount += Data.NewValue / 3;
	}
}

