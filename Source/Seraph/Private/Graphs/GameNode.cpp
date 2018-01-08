// Fill out your copyright notice in the Description page of Project Settings.

#include "GameNode.h"
#include "UnrealNetwork.h"

// Sets default values
AGameNode::AGameNode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
}




bool AGameNode::Contains(AActor* PossibleOccupant) const
{
	return PossibleOccupant == Occupant;
}

AActor* AGameNode::GetOccupant()
{
	return Occupant;
}

bool AGameNode::SetOccupant(AActor* NewOccupant)
{
	if (Occupant || NewOccupant == nullptr) return false;
	Occupant = NewOccupant;
	return true;
}

void AGameNode::RemoveOccupant()
{
	Occupant = nullptr;
}

bool AGameNode::IsEmpty() const
{
	return Occupant == nullptr;
}

void AGameNode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameNode, Occupant);
}