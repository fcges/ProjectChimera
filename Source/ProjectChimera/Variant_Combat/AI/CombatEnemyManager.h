// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/RPGAttributeSet.h"
#include "AbilitySystemInterface.h"
#include <AbilitySystemComponent.h>
//#include <mutex>
#include "CombatEnemyManager.generated.h"

UCLASS()
class PROJECTCHIMERA_API ACombatEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UAbilitySystemComponent* AbilitySystemComponent;

	/** List of actors to activate after the last enemy dies */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Spawner")
	TArray<AActor*> EnemySpawner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Spawner")
	TArray<AActor*> EliteEnemySpawner;

	int32 EnemyToSpawnCount = 0;

	int32 EliteEnemyToSpawnCount = 0;

	//std::mutex Mtx;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when player level up.
	void OnPlayerLevelUp(const FOnAttributeChangeData& Data);

	// Registers for enemy spawners to decrease enemy to spawn counters.
	void OnEnemySpawned();
	void OnEliteEnemySpawned();

	bool GetMoreEnemyToSpawn(const AActor* InEnemy);

};
