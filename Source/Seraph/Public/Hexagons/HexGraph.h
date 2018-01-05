// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexGraph.generated.h"

USTRUCT(BlueprintType)
struct FHexNode
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexNode")
		AActor* Occupant;

	FHexNode()
		:Occupant(nullptr)
	{}

	bool Contains(AActor* PossibleOccupant) const
	{
		return PossibleOccupant != nullptr && PossibleOccupant == Occupant;
	}

	bool Add(AActor* NewOccupant)
	{
		if (Occupant) return false;
		Occupant = NewOccupant;
		return true;
	}

	void RemoveOccupant()
	{
		Occupant = nullptr;
	}

	bool IsEmpty() const
	{
		return Occupant == nullptr;
	}
};

UCLASS()
class SERAPH_API AHexGraph : public AActor
{
	GENERATED_UCLASS_BODY()

private:

	int32 IndexOf(const FIntPoint& InHex) const;

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HexGraph")
		TArray<FHexNode> Nodes;

public:	

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HexGraph")
		FIntPoint Dimensions;

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		bool IsValidLocation(const FVector& InLocation) const;

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		bool IsValidHex(const FIntPoint& InHex) const;

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		void Resize(const FIntPoint& NewDimensions);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Resize(const FIntPoint& NewDimensions);

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		void InitNodes();

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		FHexNode& GetHexNode(const FIntPoint& InCoords);
	
	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		bool AddActor(AActor* ActorToAdd);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_AddActor(AActor* ActorToAdd);

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		bool MoveActor(AActor* ActorToMove, const FIntPoint& Destination);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MoveActor(AActor* ActorToMove, const FIntPoint& Destination);
};
