// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetsHexNode.generated.h"

class AActor;

// This class does not need to be modified.
UINTERFACE()
class UTargetsHexNode : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class ITargetsHexNode
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual AActor* GetTarget();
	
	virtual void SetTarget(AActor* NewTarget);
};
