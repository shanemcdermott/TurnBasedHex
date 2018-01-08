// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameGraph.generated.h"

class AGameNode;


// This class does not need to be modified.
UINTERFACE()
class UGameGraph : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IGameGraph
{
	GENERATED_BODY()

	UFUNCTION()
		virtual bool IsValidLocation(const FVector& InLocation) const;

	UFUNCTION()
		virtual AGameNode* GetGameNode(const FVector& InCoords);
	
	UFUNCTION()
		virtual bool AddActor(AActor* ActorToAdd);

	UFUNCTION()
	virtual bool MoveActor(AActor* ActorToMove, const FVector& Destination);

	UFUNCTION()
	virtual void GetNeighbors(const FVector& Center, TArray<AGameNode*>& OutNeighbors);
};
