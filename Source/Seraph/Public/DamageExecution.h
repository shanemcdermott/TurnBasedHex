// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class UDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()	
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
