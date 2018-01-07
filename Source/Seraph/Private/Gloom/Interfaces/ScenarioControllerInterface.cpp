// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioControllerInterface.h"

UScenarioControllerInterface::UScenarioControllerInterface(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{}
// Add default functionality here for any IScenarioInterface functions that are not pure virtual.

uint8 IScenarioControllerInterface::GetInitiativeValue_Implementation() const
{
	return 100;
}

FString IScenarioControllerInterface::GetCharacterName_Implementation() const
{
	return "Unknown Character";
}