// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomGameMode.h"
#include "GASCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GloomGameState.h"
#include "Hexagons/HexGraph.h"
#include "Player/GloomPlayerController.h"
#include "AI/GloomAIController.h"

struct FSortByInitiative
{
	bool operator()(const AController* A, const AController* B) const
	{
		return IScenarioControllerInterface::Execute_GetInitiativeValue(A) > IScenarioControllerInterface::Execute_GetInitiativeValue(B);
	}
};

AGloomGameMode::AGloomGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bDelayedStart = true;
}


void AGloomGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AGloomGameMode::CalcTurnOrder()
{
	Algo::Sort(GetGameState<AGloomGameState>()->ScenarioControllers, FSortByInitiative());
}

void AGloomGameMode::PerformScenarioSetup_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_StartScenarioSetup();
	
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; It++)
	{
		GS->ScenarioControllers.Add(It->Get());
	}

	GS->SetScenarioState(EScenarioState::RoundSetup);
}

void AGloomGameMode::PrepareForRound()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_StartRoundPreparation();
	Execute_OnPrepareForRound(this);
}

void AGloomGameMode::BeginRound_Implementation()
{
	CalcTurnOrder();
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_BeginRound();
	Execute_BeginTurn(this);
}

void AGloomGameMode::BeginTurn_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->CurrentTurnIndex++;
	GS->Multicast_TurnStart();
	
	AGloomAIController* TurnController = Cast<AGloomAIController>(GS->GetCurrentTurnController());
	if (TurnController)
	{
		TurnController->Execute_BeginTurn(TurnController);
	}
}

void AGloomGameMode::EndTurn_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_TurnEnd();

	if (GS->GetTurnsRemaining() <= 1)
		GS->SetScenarioState(EScenarioState::RoundCleanup);
	else
	{
		Execute_BeginTurn(this);
	}

}

void AGloomGameMode::PerformRoundCleanup_Implementation()
{
	//Decrease Elements
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_PerformRoundCleanup();
	GS->SetScenarioState(EScenarioState::RoundSetup);
}