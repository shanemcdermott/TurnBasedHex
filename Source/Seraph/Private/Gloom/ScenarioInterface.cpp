// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioInterface.h"

UScenarioInterface::UScenarioInterface(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{}
// Add default functionality here for any IScenarioInterface functions that are not pure virtual.

bool IScenarioInterface::IsReadyToStartScenario() const
{
	return true;
}

bool IScenarioInterface::IsReadyToStartRound() const
{
	return true;
}
