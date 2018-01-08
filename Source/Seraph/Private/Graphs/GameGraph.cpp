// Fill out your copyright notice in the Description page of Project Settings.

#include "GameGraph.h"
#include "GameNode.h"


bool IGameGraph::IsValidLocation(const FVector& InLocation) const
{
	return false;
}

AGameNode* IGameGraph::GetGameNode(const FVector& InCoords)
{
	return nullptr;	
}

bool IGameGraph::AddActor(AActor* ActorToAdd)
{
	return false;
}

bool IGameGraph::MoveActor(AActor* ActorToMove, const FVector& Destination)
{
	return false;
}

void IGameGraph::GetNeighbors(const FVector& Center, TArray<AGameNode*>& OutNeighbors)
{
	
}