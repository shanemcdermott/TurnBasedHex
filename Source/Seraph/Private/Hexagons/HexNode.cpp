// Fill out your copyright notice in the Description page of Project Settings.

#include "HexNode.h"
#include "UnrealNetwork.h"

// Sets default values
AHexNode::AHexNode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
}




bool AHexNode::Contains(AActor* PossibleOccupant) const
{
	return PossibleOccupant == Occupant;
}

AActor* AHexNode::GetOccupant()
{
	return Occupant;
}

bool AHexNode::SetOccupant(AActor* NewOccupant)
{
	if (Occupant || NewOccupant == nullptr) return false;
	Occupant = NewOccupant;
	return true;
}

void AHexNode::RemoveOccupant()
{
	Occupant = nullptr;
}

bool AHexNode::IsEmpty() const
{
	return Occupant == nullptr;
}

void AHexNode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHexNode, Occupant);
}