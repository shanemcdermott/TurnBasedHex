// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ScenarioInterface.h"
#include "GloomGameMode.generated.h"

class AGASCharacter;

/**
 * 
 */
UCLASS()
class SERAPH_API AGloomGameMode : public AGameMode, public IScenarioInterface
{
	GENERATED_UCLASS_BODY()


	
public:

	UPROPERTY()
		TArray<AGASCharacter*> Pawns;

	UPROPERTY()
		int32 NumActionResponsesReceived;


	virtual void PostLogin(APlayerController* NewPlayer) override;




	virtual void BeginPlay() override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	UFUNCTION()
		void RegisterPawn(AGASCharacter* NewPawn);

	void BeginScenario();

	void BeginActionSelection();

	void ReceiveActionSelect(AGASCharacter* Pawn, uint8 ActionA, uint8 ActionB);

	void CalcTurnOrder();

	UFUNCTION(BlueprintImplementableEvent, Category = "Turn")
		void OnBeginTurn(APawn* InstigatorPawn);

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
