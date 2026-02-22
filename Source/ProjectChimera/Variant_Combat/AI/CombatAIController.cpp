// Copyright Epic Games, Inc. All Rights Reserved.


#include "CombatAIController.h"
#include "Components/StateTreeAIComponent.h"
#include <Perception/AISenseConfig_Sight.h>

ACombatAIController::ACombatAIController()
{
	// create the StateTree AI Component
	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAI"));
	check(StateTreeAI);

	// ensure we start the StateTree when we possess the pawn
	bStartAILogicOnPossess = true;

	// ensure we're attached to the possessed character.
	// this is necessary for EnvQueries to work correctly
	bAttachToPawn = true;

	AiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	//Create the config object for AISensePerceptionComponent
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseConfig_Sight");

	SightConfig->SightRadius = 10000;
	SightConfig->LoseSightRadius = 10500;
	SightConfig->PeripheralVisionAngleDegrees = 60;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AiPerception->ConfigureSense(*SightConfig);
	AiPerception->SetDominantSense(SightConfig->GetSenseImplementation());

}


void ACombatAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AiPerception)
	{
		// Bind the callback
		AiPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ACombatAIController::OnTargetPerceptionUpdated);
	}
}

void ACombatAIController::OnTargetPerceptionUpdated(AActor* InActor, FAIStimulus InStimulus)
{
	if (InActor && InActor->ActorHasTag("Player")) // Assuming player has a "Player" tag
	{
		UE_LOG(LogTemp, Display, TEXT("An Actor sensed."));
		if (InStimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Sighted!"));
			// Logic: Set Blackboard key, start behavior tree, etc.
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
			// Logic: Lost sight
		}
	}
}