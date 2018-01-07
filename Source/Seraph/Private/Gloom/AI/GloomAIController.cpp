// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomAIController.h"
#include "UnrealNetwork.h"

#include "GloomGameMode.h"
#include "GloomAICharacter.h"

AGloomAIController::AGloomAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Initiative = 99;
}

void AGloomAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGloomAIController, Initiative);
}

uint8 AGloomAIController::GetInitiativeValue_Implementation() const
{
	return Initiative;
}

FString AGloomAIController::GetCharacterName_Implementation() const
{
	AGloomAICharacter* GP = Cast<AGloomAICharacter>(GetPawn());
	if (GP)
		return GP->GetName();

	return "Unknown Character";
}


void AGloomAIController::BeginTurn_Implementation()
{
	Execute_EndTurn(this);
}

void AGloomAIController::EndTurn_Implementation()
{
	if (Role == ROLE_Authority)
	{
		AGloomGameMode* GM = Cast<AGloomGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->Execute_EndTurn(GM);
		}
	}
}

void AGloomAIController::SetInitiative(uint8 Init)
{
	if (Role == ROLE_Authority)
	{
		Initiative = Init;
	}
	else
	{
		ServerSetInitiative(Init);
	}
}

bool AGloomAIController::ServerSetInitiative_Validate(uint8 Init)
{
	return true;
}

void AGloomAIController::ServerSetInitiative_Implementation(uint8 Init)
{
	SetInitiative(Init);
}