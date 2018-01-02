// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomPlayerController.h"
#include "UnrealNetwork.h"
#include "GloomPlayerState.h"
#include "GASCharacter.h"
#include "UI/GloomHUD.h"

AGloomPlayerController::AGloomPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bShowMouseCursor = true;
}

AGloomPlayerState* AGloomPlayerController::GetPlayerState()
{
	return Cast<AGloomPlayerState>(PlayerState);
}

bool AGloomPlayerController::IsReadyToStartRound() const
{
	return bIsReadyToStartRound;
}

void AGloomPlayerController::SetPawnClass(TSubclassOf<AGASCharacter> InPawnClass)
{
	if (Role == ROLE_Authority)
	{
		PawnClass = InPawnClass;
	}
	else
	{
		ServerSetPawnClass(InPawnClass);
	}
}

bool AGloomPlayerController::ServerSetPawnClass_Validate(TSubclassOf<AGASCharacter> InPawnClass)
{
	return true;
}

void AGloomPlayerController::ServerSetPawnClass_Implementation(TSubclassOf<AGASCharacter> InPawnClass)
{
	SetPawnClass(InPawnClass);
}

void AGloomPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGloomPlayerController, bIsReadyToStartRound);
	DOREPLIFETIME(AGloomPlayerController, PawnClass);
	DOREPLIFETIME(AGloomPlayerController, PlayerId);
}

void AGloomPlayerController::PerformScenarioSetup_Implementation()
{

	if (GloomHUD == nullptr)
	{
		GloomHUD = Cast<AGloomHUD>(MyHUD);
	}
	GloomHUD->Execute_PerformScenarioSetup(GloomHUD);
	
}

void AGloomPlayerController::PrepareForRound_Implementation()
{
	GloomHUD->Execute_PrepareForRound(GloomHUD);

}

void AGloomPlayerController::BeginRound_Implementation()
{
	GloomHUD->Execute_BeginRound(GloomHUD);
}

void AGloomPlayerController::BeginTurn_Implementation()
{
	GloomHUD->Execute_BeginTurn(GloomHUD);
}

void AGloomPlayerController::EndTurn_Implementation()
{
	GloomHUD->Execute_EndTurn(GloomHUD);
}

void AGloomPlayerController::PerformRoundCleanup_Implementation()
{
	GloomHUD->Execute_PerformRoundCleanup(GloomHUD);
	bIsReadyToStartRound = false;
}

AGASCharacter * AGloomPlayerController::GetGloomPawn()
{
	if (GetPawn())
	{
		return Cast<AGASCharacter>(GetPawn());
	}
	return nullptr;
}
