// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ScenarioInterface.h"
#include "GloomGameMode.generated.h"

class AGloomPlayerController;

/**
 * 
 */
UCLASS()
class SERAPH_API AGloomGameMode : public AGameMode, public IScenarioInterface
{
	GENERATED_UCLASS_BODY()

private:

		UPROPERTY()
		TArray<AGloomPlayerController*> ScenarioPlayers;
	
public:

	virtual void BeginPlay() override;

	void CalcTurnOrder();

	UFUNCTION(BlueprintNativeEvent)
		void PerformScenarioSetup();

	UFUNCTION(BlueprintNativeEvent)
		void PrepareForRound();

	UFUNCTION(BlueprintNativeEvent)
		void BeginRound();

	UFUNCTION(BlueprintNativeEvent)
		void BeginTurn();

	UFUNCTION(BlueprintNativeEvent)
		void EndTurn();

	UFUNCTION(BlueprintNativeEvent)
		void PerformRoundCleanup();
};
