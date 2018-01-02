// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASCharacter.h"
#include "GloomAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class SERAPH_API AGloomAICharacter : public AGASCharacter
{
	GENERATED_BODY()
	
	
public:

	virtual void BeginActionSelection() override;
	virtual void StartTurn() override;
	
};
