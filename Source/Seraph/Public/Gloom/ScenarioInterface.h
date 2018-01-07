// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioInterface.generated.h"



// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UScenarioInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class IScenarioInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void PerformScenarioSetup();

	UFUNCTION()
		virtual bool IsReadyToStartScenario() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void PrepareForRound();

	UFUNCTION()
		virtual bool IsReadyToStartRound() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void BeginRound();

	UFUNCTION(BlueprintNativeEvent, Category = "Scenario")
		void BeginTurn();

	UFUNCTION(BlueprintNativeEvent, Category = "Scenario")
		void EndTurn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Scenario")
		void PerformRoundCleanup();
	
};
