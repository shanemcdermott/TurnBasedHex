// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomGameMode.h"
#include "GASCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GloomGameState.h"
#include "Player/GloomPlayerController.h"

struct FSortByInitiative
{
	bool operator()(const AGASCharacter* A, const AGASCharacter* B) const
	{
		return A->Initiative > B->Initiative;
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

void AGloomGameMode::ReceiveActionSelect(AGASCharacter* Pawn, uint8 ActionA, uint8 ActionB)
{
	UE_LOG(LogTemp, Log, TEXT("Received Action Selections %d and %d from %s"), ActionA, ActionB, *Pawn->Name);

	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->ConsiderStartingRound();
}

void AGloomGameMode::CalcTurnOrder()
{
	Algo::Sort(ScenarioPawns, FSortByInitiative());
}



void AGloomGameMode::PerformScenarioSetup_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->Multicast_StartScenarioSetup();
	
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		AGASCharacter* GC = Cast<AGASCharacter>(It->Get());
		if (GC)
		{
			ScenarioPawns.Add(GC);
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
	ScenarioPawns[TurnIndex]->StartTurn();
}


void AGloomGameMode::EndTurn_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	int32 Turn = GS->CurrentTurnIndex;
	if (Turn + 1 >= ScenarioPawns.Num())
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