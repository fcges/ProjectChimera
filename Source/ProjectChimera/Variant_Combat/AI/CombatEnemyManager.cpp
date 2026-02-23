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
#include <random>
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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot access Player's ASC in ACombatEnemyManager::BeginPlay(). Please check."));
		}
	}

	for (int i = 0; i < EnemySpawner.Num(); i++)
	{
		ACombatEnemySpawner* Spawner = Cast<ACombatEnemySpawner>(EnemySpawner[i]);
		Spawner->RegisterCallback(this);
	}
	for (int i = 0; i < EliteEnemySpawner.Num(); i++)
	{
		ACombatEnemySpawner* EliteSpawner = Cast<ACombatEnemySpawner>(EliteEnemySpawner[i]);
		EliteSpawner->RegisterCallback(this);
	}
		
	NewEnemyWave();
}

// Called every frame
void ACombatEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatEnemyManager::OnEnemySpawned()
{
	//std::lock_guard<std::mutex> Lock(Mtx);
	// EnemyToSpawnCount = EnemyToSpawnCount - 1;
	EnemyAlive += 1;
}

void ACombatEnemyManager::OnEnemyKilled()
{
	EnemyAlive -= 1;
	if (EnemyAlive == 0 && EnemyToSpawnCount == 0)
	{
		NewEnemyWave();
	}
}

void ACombatEnemyManager::OnEliteEnemySpawned()
{
	//std::lock_guard<std::mutex> Lock(Mtx);
	// EliteEnemyToSpawnCount = EliteEnemyToSpawnCount - 1;
	EliteAlive += 1;
}

void ACombatEnemyManager::OnEliteEnemyKilled()
{
	EliteAlive -= 1;
}

// int ACombatEnemyManager::GetMoreEnemyToSpawn(const AActor* InEnemy)
// {
// 	if (!IsValid(InEnemy))
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Enemy is invalid "));
// 		return 0;
// 	}
// 	TArray<FName> EnemyTags = InEnemy->Tags;
// 	UE_LOG(LogTemp, Display, TEXT("First tag of the Enemy is %s"), *EnemyTags[0].ToString());
// 	if (EnemyTags[0].IsEqual(TEXT("Enemy")))
// 	{
// 		if (EnemyToSpawnCount > 0) {
// 			return true;
// 		}
// 		else if (IsValid(AbilitySystemComponent))
// 		{
// 			bool bFound = false;
// 			int32 Level = AbilitySystemComponent->GetGameplayAttributeValue(
// 					URPGAttributeSet::GetCurrentLevelAttribute(), 
// 					bFound
// 				);
// 			if (bFound)
// 			{
// 				EnemyToSpawnCount += Level * 3;
// 			}
// 			UE_LOG(LogTemp, Display, TEXT("Enemy replenished. New amount %d"), EnemyToSpawnCount);
// 			return bFound;
// 		}
// 	}
// 	if (EnemyTags[0].IsEqual(TEXT("EliteEnemy")) && EliteEnemyToSpawnCount > 0) 
// 	{
// 		return 1;
// 	}
// 	return false;
// }

void ACombatEnemyManager::OnPlayerLevelUp(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("Player level up. New level data %d"), (int32)Data.NewValue);
	// NewEnemyWave();
}

void ACombatEnemyManager::NewEnemyWave()
{
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot access Player's ASC. Please check."));
		return;
	}
	int32 Level = 1;
	if (IsValid(AbilitySystemComponent))
	{
		bool bFound = false;
		Level = AbilitySystemComponent->GetGameplayAttributeValue(URPGAttributeSet::GetCurrentLevelAttribute(), bFound);
		if (bFound)
		{
			EnemyToSpawnCount += Level * 3;
		}
		if (Level % 3 == 0)
		{
			EliteEnemyToSpawnCount += Level / 3;
		}
	}
	std::random_device Random_Device;
	std::mt19937 Gen(Random_Device());
	for (AActor* Spawner : EnemySpawner)
	{
		ACombatEnemySpawner* CastedSpawner = Cast<ACombatEnemySpawner>(Spawner);
		if (CastedSpawner)
		{
			std::uniform_real_distribution<> Dis(1.0, Level * 5.0);
			const int EnemyRandomResult = std::min(static_cast<int>(Dis(Gen)), EnemyToSpawnCount);
			CastedSpawner->ReceiveEnemyFromManager(EnemyRandomResult);
			EnemyToSpawnCount -= EnemyRandomResult;
			UE_LOG(LogTemp, Display, TEXT("Distributed %d enemy(enemies) to a CombatEnemySpawner, "
								 "remaining EnemyToSpawnCount: %d"), EnemyRandomResult, EnemyToSpawnCount);
			if (Spawner == EnemySpawner.Last() && EnemyToSpawnCount > 0)
			{
				int32 Remains = EnemyToSpawnCount;
				CastedSpawner->ReceiveEnemyFromManager(Remains);
				EnemyToSpawnCount = 0;
				UE_LOG(LogTemp, Display, TEXT("Distributed additional %d enemy(enemies) to the last CombatEnemySpawner, "
									 "remaining EnemyToSpawnCount: %d"), Remains, EnemyToSpawnCount);				
			}
		}
	}
	for (AActor* EliteSpawner: EliteEnemySpawner)
	{
		ACombatEnemySpawner* CastedEliteSpawner = Cast<ACombatEnemySpawner>(EliteSpawner);
		if (CastedEliteSpawner)
		{
			std::uniform_real_distribution<> Dis(0.0, Level * 3.0);
			const int EliteEnemyRandomResult = std::min(static_cast<int>(Dis(Gen)), EliteEnemyToSpawnCount);
			CastedEliteSpawner->ReceiveEnemyFromManager(EliteEnemyRandomResult);
			EliteEnemyToSpawnCount -= EliteEnemyRandomResult;
			UE_LOG(LogTemp, Display, TEXT("Distributed %d elite enemy(enemies) to a CombatEnemySpawner, "
								 "remaining EliteEnemyToSpawnCount: %d"), EliteEnemyRandomResult, EliteEnemyToSpawnCount);
		}
	}
}
