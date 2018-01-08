// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameNode.generated.h"

UCLASS()
class SERAPH_API AGameNode : public AActor
{
	GENERATED_UCLASS_BODY()
private:

	UPROPERTY(Replicated)
		AActor* Occupant;

public:	

	UFUNCTION(BlueprintCallable, Category = "HexNode")
	bool Contains(AActor* PossibleOccupant) const;
	
	UFUNCTION(BlueprintCallable, Category = "HexNode")
		AActor* GetOccupant();

	UFUNCTION(BlueprintCallable, Category = "HexNode")
		bool SetOccupant(AActor* NewOccupant);

	UFUNCTION(BlueprintCallable, Category = "HexNode")
		void RemoveOccupant();

	UFUNCTION(BlueprintCallable, Category = "HexNode")
		bool IsEmpty() const;
	
	
};
