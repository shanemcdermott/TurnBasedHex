// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomGameMode.h"
#include "GASCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GloomGameState.h"
#include "Player/GloomPlayerController.h"

struct FSortByInitiative
{
	bool operator()(const AGloomPlayerController* A, const AGloomPlayerController* B) const
	{
		return A->GetInitiativeValue() > B->GetInitiativeValue();
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
	Algo::Sort(ScenarioPlayers, FSortByInitiative());
}



void AGloomGameMode::PerformScenarioSetup_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_StartScenarioSetup();
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(It->Get());
		if (PC)
		{
			ScenarioPlayers.Add(PC);
		}
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
	BeginTurn();
}

void AGloomGameMode::BeginTurn_Implementation()
{
	int32 TurnIndex = GetGameState<AGloomGameState>()->CurrentTurnIndex;
	ScenarioPlayers[TurnIndex]->Execute_BeginTurn(ScenarioPlayers[TurnIndex]);
}


void AGloomGameMode::EndTurn_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	int32 Turn = GS->CurrentTurnIndex;
	if (Turn + 1 >= ScenarioPlayers.Num())
		GS->SetScenarioState(EScenarioState::RoundCleanup);
	else
	{
		GS->CurrentTurnIndex++;
		BeginTurn();
	}

}

void AGloomGameMode::PerformRoundCleanup_Implementation()
{
	//Decrease Elements
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_PerformRoundCleanup();
	GS->SetScenarioState(EScenarioState::RoundSetup);
}