// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GloomGameState.generated.h"

UENUM(BlueprintType)
enum class EScenarioState : uint8
{
	WaitingToStart,
	ScenarioSetup,
	RoundSetup,
	MidRound,
	RoundCleanup,
	ScenarioCompleted,
	ScenarioFailed

};
/**
 * 
 */
UCLASS()
class SERAPH_API AGloomGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

private:

	void ProcessScenarioStateChange();

public:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Scenario")
		EScenarioState ScenarioState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Scenario")
		int32 RoundNumber;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Scenario")
		int32 CurrentTurnIndex;

	UFUNCTION()
		AGloomPlayerController* GetGloomPlayer(int32 id);

	UFUNCTION()
		void SetScenarioState(EScenarioState State);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetScenarioState(EScenarioState State);

	UFUNCTION(BlueprintCallable, Category = "Scenario|Gloom")
		void ConsiderStartingScenario();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerConsiderStartingScenario();


	UFUNCTION(BlueprintCallable, Category = "Scenario|Gloom")
		void ConsiderStartingRound();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerConsiderStartingRound();
};
