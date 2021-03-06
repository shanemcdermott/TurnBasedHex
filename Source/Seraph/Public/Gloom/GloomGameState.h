// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GloomGameState.generated.h"

class AHexGraph;
class AController;

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
		TArray<AController*> ScenarioControllers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Scenario")
		EScenarioState ScenarioState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Scenario")
		int32 RoundNumber;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Scenario")
		int32 CurrentTurnIndex;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Scenario")
		AHexGraph* HexGraph;

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

	//////GETTERS

	UFUNCTION(BlueprintCallable, Category = "Scenario|Gloom")
		AController* GetCurrentTurnController() const;

	UFUNCTION(BlueprintCallable, Category = "Scenario|Gloom")
		bool IsLocalPlayersTurn() const;

	/*Returns the number of characters that still need to take their turn this round (including the current turn).*/
	UFUNCTION(BlueprintCallable, Category = "Scenario|Gloom")
		int32 GetTurnsRemaining() const;

	//////MULTICASTS

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_StartScenarioSetup();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_StartRoundPreparation();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_BeginRound();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_TurnStart();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_TurnEnd();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_PerformRoundCleanup();

	////////HEX GRAPH
	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		void SetHexGraph(AHexGraph* InGraph);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetHexGraph(AHexGraph* InGraph);
};
