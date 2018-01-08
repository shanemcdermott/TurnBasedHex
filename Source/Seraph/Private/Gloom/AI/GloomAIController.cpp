// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomAIController.h"
#include "UnrealNetwork.h"

#include "GloomGameMode.h"
#include "GASCharacter.h"
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

AGASCharacter * AGloomAIController::GetGloomPawn() const
{
	if (GetPawn())
	{
		return Cast<AGASCharacter>(GetPawn());
	}
	return nullptr;
}

FString AGloomAIController::GetCharacterName_Implementation() const
{
	AGASCharacter* GP = Cast<AGASCharacter>(GetPawn());
	if (GP)
		return GP->GetName();

	return "Unknown Character";
}

uint8 AGloomAIController::GetInitiativeValue_Implementation() const
{
	return Initiative;
}

void AGloomAIController::PrepareForRound()
{
	SelectActionForTurn();
	Execute_OnPrepareForRound(this);
}

void AGloomAIController::BeginTurn_Implementation()
{
	AGASCharacter* GloomPawn = GetGloomPawn();
	if (GloomPawn)
		GloomPawn->TryActivateAbility(GloomPawn->LeadingActionIndex, true);
	//Execute_EndTurn(this);
}

void AGloomAIController::EndCharacterTurn()
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

void AGloomAIController::SelectActionForTurn()
{
	AGASCharacter* GloomPawn = GetGloomPawn();
	if (GloomPawn)
	{
		int32 ActionID = FMath::RandRange(0, GloomPawn->AbilityList.Num()-1);
		GloomPawn->SetLeadingAction(ActionID);
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