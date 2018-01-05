// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGraph.h"
#include "HexagonLib.h"
#include "UnrealNetwork.h"

// Sets default values
AHexGraph::AHexGraph(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	Dimensions = FIntPoint(4, 5);
	InitNodes();
}

int32 AHexGraph::IndexOf(const FIntPoint& InHex) const
{
	return InHex.X * Dimensions.X + InHex.Y;
}

bool AHexGraph::IsValidLocation(const FVector& InLocation) const
{
	FIntPoint HexPoint;
	UHexagonLib::WorldToAxial(InLocation, HexPoint);
	return IsValidHex(HexPoint);
}

bool AHexGraph::IsValidHex(const FIntPoint& InHex) const
{
	return InHex.X >= 0 && InHex.X < Dimensions.X && InHex.Y >= 0 && InHex.Y < Dimensions.Y;
}

void AHexGraph::Resize(const FIntPoint& NewDimensions)
{
	if (Role == ROLE_Authority)
	{
		Dimensions = NewDimensions;
		InitNodes();
	}
	else
	{
		Server_Resize(NewDimensions);
	}
}

bool AHexGraph::Server_Resize_Validate(const FIntPoint& NewDimensions)
{
	return true;
}

void AHexGraph::Server_Resize_Implementation(const FIntPoint& NewDimensions)
{
	Resize(NewDimensions);
}

void AHexGraph::InitNodes()
{
	if (Nodes.Num() != UHexagonLib::LengthOf(Dimensions))
	{
		Nodes.SetNumZeroed(UHexagonLib::LengthOf(Dimensions));
	}
}

FHexNode& AHexGraph::GetHexNode(const FIntPoint& InCoords)
{
	return Nodes[IndexOf(InCoords)];
}

bool AHexGraph::AddActor(AActor* ActorToAdd)
{
	if (Role == ROLE_Authority && ActorToAdd != nullptr)
	{
		FIntPoint AxialHex;
		UHexagonLib::WorldToAxial(ActorToAdd->GetActorLocation(), AxialHex);

		if (GetHexNode(AxialHex).Add(ActorToAdd))
		{
			FVector WorldCoords;
			UHexagonLib::AxialToWorld(AxialHex.X, AxialHex.Y, WorldCoords);
			ActorToAdd->SetActorLocation(WorldCoords);
			return true;
		}
		return false;
	}
	else
	{
		Server_AddActor(ActorToAdd);
	}
	return false;
}

bool AHexGraph::Server_AddActor_Validate(AActor* ActorToAdd)
{
	return ActorToAdd != nullptr && IsValidLocation(ActorToAdd->GetActorLocation());
}

void AHexGraph::Server_AddActor_Implementation(AActor* ActorToAdd)
{
	AddActor(ActorToAdd);
}

bool AHexGraph::MoveActor(AActor* ActorToMove, const FIntPoint& Destination)
{
	if (ActorToMove == nullptr || !IsValidHex(Destination)) return false;

	if (Role == ROLE_Authority)
	{
		FIntPoint StartingHex;
		UHexagonLib::WorldToAxial(ActorToMove->GetActorLocation(), StartingHex);
		FHexNode HexNode = GetHexNode(StartingHex);
		if (HexNode.Contains(ActorToMove)) HexNode.RemoveOccupant();

		return GetHexNode(Destination).Add(ActorToMove);
	}
	else if(IsValidHex(Destination))
	{
		Server_MoveActor(ActorToMove, Destination);
		return true;
	}
	return false;
}

bool AHexGraph::Server_MoveActor_Validate(AActor* ActorToMove, const FIntPoint& Destination)
{
	return true;
}

void AHexGraph::Server_MoveActor_Implementation(AActor* ActorToMove, const FIntPoint& Destination)
{
	MoveActor(ActorToMove, Destination);
}

void AHexGraph::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHexGraph, Nodes);
	DOREPLIFETIME(AHexGraph, Dimensions);
}