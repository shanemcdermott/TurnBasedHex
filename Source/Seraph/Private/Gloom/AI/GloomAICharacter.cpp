// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomAICharacter.h"




void AGloomAICharacter::BeginActionSelection()
{
	SubmitActionSelection(FMath::RandRange(5,98), FMath::RandRange(5,99));
}

void AGloomAICharacter::StartTurn()
{
	Super::StartTurn();
	UE_LOG(LogTemp, Log, TEXT("Trying to end turn..."));
	EndTurn();
}
