// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomGameMode.h"
#include "GASCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GloomGameState.h"
#include "Hexagons/HexGraph.h"
#include "Player/GloomPlayerController.h"

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
		GS->HexGraph->AddActor(It->Get()->GetPawn());
	}

	GS->SetScenarioState(EScenarioState::RoundSetup);
}

void AGloomGameMode::PrepareForRound_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_StartRoundPreparation();
}

void AGloomGameMode::BeginRound_Implementation()
{
	CalcTurnOrder();
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_BeginRound();
	GS->Multicast_StartTurn(0);
}

void AGloomGameMode::BeginTurn_Implementation()
{
	
}

void AGloomGameMode::EndTurn_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	int32 Turn = GS->CurrentTurnIndex;
	if (Turn + 1 >= GS->ScenarioControllers.Num())
		GS->SetScenarioState(EScenarioState::RoundCleanup);
	else
	{
		GS->Multicast_StartTurn(Turn + 1);
	}

}

void AGloomGameMode::PerformRoundCleanup_Implementation()
{
	//Decrease Elements
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_PerformRoundCleanup();
	GS->SetScenarioState(EScenarioState::RoundSetup);
}