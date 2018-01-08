// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ScenarioInterface.h"
#include "Interfaces/ScenarioControllerInterface.h"
#include "GloomAIController.generated.h"

class AGASCharacter;
class AGloomAICharacter;
/**
 * 
 */
UCLASS()
class SERAPH_API AGloomAIController : public AAIController, public IScenarioInterface, public IScenarioControllerInterface
{
	GENERATED_UCLASS_BODY()
	
public:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Scenario")
		uint8 Initiative;

	UFUNCTION(BlueprintCallable, Category = "Character")
		AGASCharacter* GetGloomPawn() const;
	
	virtual FString GetCharacterName_Implementation() const override;
	virtual uint8 GetInitiativeValue_Implementation() const override;

	virtual void PrepareForRound() override;
	virtual void BeginTurn_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Turns")
		void EndCharacterTurn();

	virtual void EndTurn_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Initiative")
		void SetInitiative(uint8 Init);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetInitiative(uint8 Init);

	UFUNCTION()
		virtual void SelectActionForTurn();
};
