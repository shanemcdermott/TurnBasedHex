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
	
public:

	virtual void BeginPlay() override;

	void CalcTurnOrder();

	UFUNCTION(BlueprintNativeEvent)
		void PerformScenarioSetup();

	UFUNCTION()
		virtual void PrepareForRound() override;

	UFUNCTION(BlueprintNativeEvent)
		void BeginRound();

	virtual void BeginTurn_Implementation() override;
	virtual void EndTurn_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
		void PerformRoundCleanup();
};
