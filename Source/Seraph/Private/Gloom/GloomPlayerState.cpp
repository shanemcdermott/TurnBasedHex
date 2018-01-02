// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomPlayerState.h"
#include "Gloom/Player/GloomPlayerController.h"
#include "Gloom/GloomGameState.h"
#include "UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void AGloomPlayerState::SetReadyToStart(bool bIsReady)
{
	if (Role == ROLE_Authority)
	{
		bIsReadyToStart = bIsReady;
	}
	else
	{
		ServerSetReadyToStart(bIsReady);
	}
}

bool AGloomPlayerState::ServerSetReadyToStart_Validate(bool bIsReady)
{
	return true;
}

void AGloomPlayerState::ServerSetReadyToStart_Implementation(bool bIsReady)
{
	SetReadyToStart(bIsReady);
}

void AGloomPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGloomPlayerState, bIsReadyToStart);
	DOREPLIFETIME(AGloomPlayerState, Controller);
}