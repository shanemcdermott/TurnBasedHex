// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioInterface.generated.h"



// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UScenarioInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SERAPH_API IScenarioInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void PerformScenarioSetup();

	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void PrepareForRound();

	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void BeginRound();

	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void BeginTurn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void EndTurn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void PerformRoundCleanup();
	
};
