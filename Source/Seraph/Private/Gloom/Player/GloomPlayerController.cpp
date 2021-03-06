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

void AGloomPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGloomPlayerController, bIsReadyToStartScenario);
	DOREPLIFETIME(AGloomPlayerController, bIsReadyToStartRound);
	DOREPLIFETIME(AGloomPlayerController, Initiative);
	DOREPLIFETIME(AGloomPlayerController, PawnClass);
	DOREPLIFETIME(AGloomPlayerController, PlayerId);
}

uint8 AGloomPlayerController::GetInitiativeValue_Implementation() const
{
	return Initiative;
}

FString AGloomPlayerController::GetCharacterName_Implementation() const
{
	AGASCharacter* GP = Cast<AGASCharacter>(GetPawn());
	if (GP)
		return GP->GetName();

	return "Unknown Character";
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


void AGloomPlayerController::PerformScenarioSetup_Implementation()
{

	if (GloomHUD == nullptr)
	{
		GloomHUD = Cast<AGloomHUD>(MyHUD);
	}
	GloomHUD->Execute_PerformScenarioSetup(GloomHUD);
	
}

void AGloomPlayerController::PrepareForRound()
{
	GloomHUD->PrepareForRound();

	Execute_OnPrepareForRound(this);
}

void AGloomPlayerController::BeginRound_Implementation()
{
	GloomHUD->Execute_BeginRound(GloomHUD);
}

void AGloomPlayerController::AnnounceTurnStart()
{
	AGloomGameState* GS = Cast<AGloomGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		AController* TurnController = GS->GetCurrentTurnController();
		if (this == TurnController)
		{
			Execute_BeginTurn(this);
		}
		else if(GloomHUD)
		{
			IScenarioControllerInterface* ISC = Cast<IScenarioControllerInterface>(TurnController);
			if (ISC)
			{
				FString Announcement = FString::Printf(TEXT("%s's Turn"), *ISC->Execute_GetCharacterName(TurnController));
				GloomHUD->OnReceiveAnnouncement(Announcement);
			}
		}
	}
}

void AGloomPlayerController::BeginTurn_Implementation()
{
	if (GloomHUD)
			GloomHUD->Execute_BeginTurn(GloomHUD);
}

void AGloomPlayerController::AnnounceTurnEnd()
{
	if (GloomHUD)
	{
		GloomHUD->OnTurnEnd();
	}
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
	if(IsLocalController())
		GloomHUD->Execute_PerformRoundCleanup(GloomHUD);
	else if (Role == ROLE_Authority)
	{
		Client_PerformRoundCleanup();
	}

	SetReadyToStartRound(false);
}

void AGloomPlayerController::Client_PerformRoundCleanup_Implementation()
{
	GloomHUD->Execute_PerformRoundCleanup(GloomHUD);
}

AGASCharacter * AGloomPlayerController::GetGloomPawn() const
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