// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomGameMode.h"
#include "GASCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GloomGameState.h"
#include "GloomPlayerState.h"
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


void AGloomGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (Role == ROLE_Authority)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(NewPlayer);
		if (PC)
		{
			PC->PlayerId = GameState->PlayerArray.Num();
			AGloomPlayerState* PS = PC->GetPlayerState();
			if (PS)
			{
				PS->Controller = PC;
			}
		}
	}
}

void AGloomGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AGloomGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	AGloomPlayerController* PC = Cast<AGloomPlayerController>(NewPlayer);
	if (PC)
	{
		AGASCharacter* GloomPawn = Cast<AGASCharacter>(PC->GetPawn());
		if (GloomPawn)
		{
			RegisterPawn(GloomPawn);
		}
	}
}

void AGloomGameMode::RegisterPawn(AGASCharacter* NewPawn)
{
	if (Pawns.Contains(NewPawn)) return;

	Pawns.Add(NewPawn);
}

void AGloomGameMode::BeginScenario()
{
	BeginActionSelection();
}

void AGloomGameMode::BeginActionSelection()
{
	UE_LOG(LogTemp, Log, TEXT("Starting Action Selection Phase"));
	NumActionResponsesReceived = 0;
	for (int i = 0; i < Pawns.Num(); i++)
	{
		Pawns[i]->BeginActionSelection();
	}
}

void AGloomGameMode::ReceiveActionSelect(AGASCharacter* Pawn, uint8 ActionA, uint8 ActionB)
{
	UE_LOG(LogTemp, Log, TEXT("Received Action Selections %d and %d from %s"), ActionA, ActionB, *Pawn->Name);
	NumActionResponsesReceived++;
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->ConsiderStartingRound();
}

void AGloomGameMode::CalcTurnOrder()
{
	Algo::Sort(Pawns, FSortByInitiative());
}



void AGloomGameMode::PerformScenarioSetup_Implementation()
{
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerState>(GameState->PlayerArray[i])->Controller;
		if (PC)
		{
			RegisterPawn(PC->GetGloomPawn());
			PC->Execute_PerformScenarioSetup(PC);
		}
	}
	
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	GS->SetScenarioState(EScenarioState::RoundSetup);
}

void AGloomGameMode::PrepareForRound_Implementation()
{
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AGloomPlayerController* PC = GS->GetGloomPlayer(i);
		if (PC)
		{
			PC->Execute_PrepareForRound(PC);
		}
	}
}

void AGloomGameMode::BeginRound_Implementation()
{
	CalcTurnOrder();
	BeginTurn();
}

void AGloomGameMode::BeginTurn_Implementation()
{
	int32 TurnIndex = GetGameState<AGloomGameState>()->CurrentTurnIndex;
	Pawns[TurnIndex]->StartTurn();
	OnBeginTurn(Pawns[TurnIndex]);
}


void AGloomGameMode::EndTurn_Implementation()
{
	int32 Turn = GetGameState<AGloomGameState>()->CurrentTurnIndex;
	if (Turn + 1 >= Pawns.Num())
		GetGameState<AGloomGameState>()->SetScenarioState(EScenarioState::RoundCleanup);
	else
	{
		GetGameState<AGloomGameState>()->CurrentTurnIndex++;
		BeginTurn();
	}

}

void AGloomGameMode::PerformRoundCleanup_Implementation()
{
	//Decrease Elements
	AGloomGameState* GS = GetGameState<AGloomGameState>();
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AGloomPlayerController* PC = GS->GetGloomPlayer(i);
		if (PC)
		{
			PC->Execute_PerformRoundCleanup(PC);
		}
	}
	GS->SetScenarioState(EScenarioState::RoundSetup);
}