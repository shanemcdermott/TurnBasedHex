// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomGameState.h"
#include "UnrealNetwork.h"
#include "GloomGameMode.h"
#include "Player/GloomPlayerController.h"
#include "Engine/World.h"
#include "Hexagons/HexGraph.h"

AGloomGameState::AGloomGameState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ScenarioState = EScenarioState::WaitingToStart;
	RoundNumber = 0;
}

void AGloomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGloomGameState, ScenarioControllers);
	DOREPLIFETIME(AGloomGameState, ScenarioState);
	DOREPLIFETIME(AGloomGameState, RoundNumber);
	DOREPLIFETIME(AGloomGameState, CurrentTurnIndex);
	DOREPLIFETIME(AGloomGameState, HexGraph);
}

AController* AGloomGameState::GetCurrentTurnController() const
{
	return ScenarioControllers[CurrentTurnIndex];
}

bool AGloomGameState::IsLocalPlayersTurn() const
{
	 return GetCurrentTurnController() == GetWorld()->GetFirstPlayerController();
}

int32 AGloomGameState::GetTurnsRemaining() const
{
	return ScenarioControllers.Num() - CurrentTurnIndex;
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
			GM->Execute_PerformScenarioSetup(GM);
			break;
		case EScenarioState::RoundSetup:
			UE_LOG(LogTemp, Log, TEXT("Preparing for Round %d"), RoundNumber + 1);
			GM->PrepareForRound();
			break;
		case EScenarioState::MidRound:
			RoundNumber++;
			CurrentTurnIndex = 0;
			UE_LOG(LogTemp, Log, TEXT("Starting Round %d"), RoundNumber);
			GM->Execute_BeginRound(GM);
			break;
		case EScenarioState::RoundCleanup:
			UE_LOG(LogTemp, Log, TEXT("End of Round %d"), RoundNumber);
			GM->Execute_PerformRoundCleanup(GM);
			break;
		}
	}
}

void AGloomGameState::ConsiderStartingScenario()
{

	if (Role == ROLE_Authority)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
		{
			IScenarioInterface* PC = Cast<IScenarioInterface>(It->Get());
			if (PC == nullptr || PC->IsReadyToStartScenario() == false)
			{
				UE_LOG(LogTemp, Log, TEXT("Not all players are ready to start the scenario."));
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
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
		{
			IScenarioInterface* PC = Cast<IScenarioInterface>(It->Get());
			if (PC == nullptr || PC->IsReadyToStartRound() == false)
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

void AGloomGameState::SetHexGraph(AHexGraph* InGraph)
{
	if (Role == ROLE_Authority)
	{
		HexGraph = InGraph;
		HexGraph->SetOwner(this);
	}
}

void AGloomGameState::Server_SetHexGraph_Implementation(AHexGraph* InGraph)
{
	SetHexGraph(InGraph);
}

bool AGloomGameState::Server_SetHexGraph_Validate(AHexGraph* InGraph)
{
	return InGraph != nullptr;
}

void AGloomGameState::Multicast_PerformRoundCleanup_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->Execute_PerformRoundCleanup(PC);
		}
	}
}

void AGloomGameState::Multicast_BeginRound_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->Execute_BeginRound(PC);
		}
	}
}

void AGloomGameState::Multicast_TurnStart_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->AnnounceTurnStart();
		}
	}
}

void AGloomGameState::Multicast_TurnEnd_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->AnnounceTurnEnd();
		}
	}
}

void AGloomGameState::Multicast_StartRoundPreparation_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->PrepareForRound();
		}
	}
}

void AGloomGameState::Multicast_StartScenarioSetup_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->Execute_PerformScenarioSetup(PC);
		}
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

