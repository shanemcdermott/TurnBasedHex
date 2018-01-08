// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graphs/GameGraph.h"
#include "HexGraph.generated.h"

class AHexNode;


UCLASS()
class SERAPH_API AHexGraph : public AActor
{
	GENERATED_UCLASS_BODY()

private:

	int32 IndexOf(const FIntPoint& InHex) const;
	int32 IndexOf(const FVector2D& InHex) const;
protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HexGraph")
		TArray<AHexNode*> Nodes;

public:	

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HexGraph")
		FIntPoint Dimensions;

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		bool IsValidLocation(const FVector& InLocation) const;

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		bool IsValidHex(const FIntPoint& InHex) const;

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		AHexNode* GetNode(const FVector2D& InCoords);

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		AHexNode* GetHexNode(const FIntPoint& InCoords);
	
	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		bool AddActor(AActor* ActorToAdd);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_AddActor(AActor* ActorToAdd);

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		bool MoveActor(AActor* ActorToMove, const FIntPoint& Destination);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MoveActor(AActor* ActorToMove, const FIntPoint& Destination);

	UFUNCTION(BlueprintCallable, Category = "HexGraph")
		void GetNeighbors(const FVector& Center, TArray<AHexNode*>& OutNeighbors);
};
