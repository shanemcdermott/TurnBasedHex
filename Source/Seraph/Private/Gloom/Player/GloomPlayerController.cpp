// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomPlayerController.h"
#include "UnrealNetwork.h"
#include "GASCharacter.h"
#include "UI/GloomHUD.h"
#include "GloomGameState.h"

AGloomPlayerController::AGloomPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bShowMouseCursor = true;
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
	AGASCharacter* GC = GetGloomPawn();
	if (GC)
	{
		GC->AddTag(FGameplayTag(TEXT("Gloom.CurrentTurn")));
	}
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

void AGloomPlayerController::Server_SetReadyToStartScenario_Implementation(bool bIsReady)
{
	SetReadyToStartScenario(bIsReady);
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

bool AGloomPlayerController::Server_SetReadyToStartScenario_Validate(bool bIsReady)
{
	return true;
}