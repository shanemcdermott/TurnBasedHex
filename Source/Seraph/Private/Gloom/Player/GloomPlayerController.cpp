// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomPlayerController.h"
#include "UnrealNetwork.h"
#include "GASCharacter.h"
#include "UI/GloomHUD.h"
#include "GloomGameState.h"
#include "GloomGameMode.h"
#include "GameplayTagContainer.h"


AGloomPlayerController::AGloomPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	Initiative = 99;
}

bool AGloomPlayerController::IsReadyToStartScenario() const
{
	return bIsReadyToStartScenario;
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

	DOREPLIFETIME(AGloomPlayerController, bIsReadyToStartScenario);
	DOREPLIFETIME(AGloomPlayerController, bIsReadyToStartRound);
	DOREPLIFETIME(AGloomPlayerController, Initiative);
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
	if (!IsLocalController())
		Client_BeginTurn();
	else
	{
		if (GloomHUD)
			GloomHUD->Execute_BeginTurn(GloomHUD);
	}

}

void AGloomPlayerController::Client_BeginTurn_Implementation()
{
	BeginTurn();
}

void AGloomPlayerController::EndTurn_Implementation()
{
	if (IsLocalController())
	{
		GloomHUD->Execute_EndTurn(GloomHUD);
	}
	else if(Role == ROLE_Authority)
	{
		Client_EndTurn();
	}
	if (Role == ROLE_Authority)
	{
		AGloomGameMode* GM = Cast<AGloomGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->Execute_EndTurn(GM);
		}
	}

}

void AGloomPlayerController::Client_EndTurn_Implementation()
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


void AGloomPlayerController::SetReadyToStartScenario(bool bIsReady)
{
	if (Role == ROLE_Authority)
	{
		bIsReadyToStartScenario = bIsReady;
		if (bIsReadyToStartScenario)
		{
			AGloomGameState* GS = Cast<AGloomGameState>(GetWorld()->GetGameState());
			if (GS)
			{
				GS->ConsiderStartingScenario();
			}
		}
	}
	else
	{
		Server_SetReadyToStartScenario(bIsReady);
	}
}

void AGloomPlayerController::Server_SetReadyToStartScenario_Implementation(bool bIsReady)
{
	SetReadyToStartScenario(bIsReady);
}

bool AGloomPlayerController::Server_SetReadyToStartScenario_Validate(bool bIsReady)
{
	return true;
}

void AGloomPlayerController::SetReadyToStartRound(bool bIsReady)
{
	if (Role == ROLE_Authority)
	{
		bIsReadyToStartRound = bIsReady;
		AGloomGameState* GS = GetWorld()->GetGameState<AGloomGameState>();
		if (GS)
		{
			GS->ConsiderStartingRound();
		}
	}
	else
	{
		Server_SetReadyToStartRound(bIsReady);
	}
}

bool AGloomPlayerController::Server_SetReadyToStartRound_Validate(bool bIsReady)
{
	return true;
}

void AGloomPlayerController::Server_SetReadyToStartRound_Implementation(bool bIsReady)
{
	SetReadyToStartRound(bIsReady);
}

uint8 AGloomPlayerController::GetInitiativeValue() const
{
	return Initiative;
}

void AGloomPlayerController::SetInitiative(uint8 Init)
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

bool AGloomPlayerController::ServerSetInitiative_Validate(uint8 Init)
{
	return true;
}

void AGloomPlayerController::ServerSetInitiative_Implementation(uint8 Init)
{
	SetInitiative(Init);
}