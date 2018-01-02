// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomGameState.h"
#include "UnrealNetwork.h"
#include "GloomGameMode.h"
#include "GloomPlayerState.h"
#include "Player/GloomPlayerController.h"

AGloomGameState::AGloomGameState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ScenarioState = EScenarioState::WaitingToStart;
	RoundNumber = 0;
}

AGloomPlayerController* AGloomGameState::GetGloomPlayer(int32 id)
{
	return Cast<AGloomPlayerState>(PlayerArray[id])->Controller;
}

void AGloomGameState::ProcessScenarioStateChange()
{
	AGloomGameMode* GM = Cast<AGloomGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		switch (ScenarioState)
		{
		case EScenarioState::ScenarioSetup:
			UE_LOG(LogTemp, Log, TEXT("Starting Scenario..."));
			GM->PerformScenarioSetup();
			break;
		case EScenarioState::RoundSetup:
			UE_LOG(LogTemp, Log, TEXT("Preparing for Round %d"), RoundNumber + 1);
			GM->PrepareForRound();
			break;
		case EScenarioState::MidRound:
			RoundNumber++;
			CurrentTurnIndex = 0;
			UE_LOG(LogTemp, Log, TEXT("Starting Round %d"), RoundNumber);
			GM->BeginRound();
			break;
		case EScenarioState::RoundCleanup:
			UE_LOG(LogTemp, Log, TEXT("End of Round %d"), RoundNumber);
			GM->PerformRoundCleanup();
			break;
		}
	}
}

void AGloomGameState::ConsiderStartingScenario()
{

	if (Role == ROLE_Authority)
	{

		for (int i = 0; i < PlayerArray.Num(); i++)
		{
			AGloomPlayerState* PS = Cast<AGloomPlayerState>(PlayerArray[i]);
			if (PS == nullptr || !PS->IsReadyToStart())
			{
				return;
			}
		}

		AGloomGameMode* GM = Cast<AGloomGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			UE_LOG(LogTemp, Log, TEXT("Starting Match..."));
			GM->StartMatch();
			SetScenarioState(EScenarioState::ScenarioSetup);
		}
	}
	else
	{
		ServerConsiderStartingScenario();
	}

}

void AGloomGameState::ConsiderStartingRound()
{
	if (Role == ROLE_Authority)
	{
		for (int i = 0; i < PlayerArray.Num(); i++)
		{
			if (!GetGloomPlayer(i)->IsReadyToStartRound())
			{
				UE_LOG(LogTemp, Log, TEXT("Not all players are ready to start the round."));
				return;
			}
		}

		SetScenarioState(EScenarioState::MidRound);
	}
	else
	{
		ServerConsiderStartingRound();
	}

}

bool AGloomGameState::ServerConsiderStartingRound_Validate()
{
	return true;
}

void AGloomGameState::ServerConsiderStartingRound_Implementation()
{
	ConsiderStartingRound();
}

bool AGloomGameState::ServerConsiderStartingScenario_Validate()
{
	return true;
}

void AGloomGameState::ServerConsiderStartingScenario_Implementation()
{
	ConsiderStartingScenario();
}

void AGloomGameState::SetScenarioState(EScenarioState State)
{
	if (Role == ROLE_Authority)
	{
		ScenarioState = State;
		ProcessScenarioStateChange();
	}
	else
	{
		ServerSetScenarioState(State);
	}
}

void AGloomGameState::ServerSetScenarioState_Implementation(EScenarioState State)
{
	SetScenarioState(State);
}

bool AGloomGameState::ServerSetScenarioState_Validate(EScenarioState State)
{
	return true;
}

void AGloomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGloomGameState, ScenarioState);
	DOREPLIFETIME(AGloomGameState, RoundNumber);
	DOREPLIFETIME(AGloomGameState, CurrentTurnIndex);
}