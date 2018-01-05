// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexagonLib.generated.h"

UENUM(BlueprintType)
enum class EAxialDirection : uint8
{
	Right,
	ForwardRight,
	ForwardLeft,
	Left,
	BackwardLeft,
	BackwardRight
};

/**
 * 
 */
UCLASS()
class UHexagonLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static const FVector2D HexSize;
	static const FVector2D HexHalfSize;
	static const FIntPoint AxialForwardRight;
	static const FIntPoint AxialRight;
	static const FIntPoint AxialBackwardRight;
	static const FIntPoint AxialBackwardLeft;
	static const FIntPoint AxialLeft;
	static const FIntPoint AxialForwardLeft;
	static const TArray<FIntPoint> AxialDirections;

	///////CONSTANTS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Constants")
	static float Sqrt3Div2();
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Constants")
		static FVector2D GetHexSize();
	
	///////CONVERSIONS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Conversions")
		static void AxialToWorld(int32 X, int32 Y, FVector& OutWorldCoords);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Conversions")
		static void WorldToAxial(const FVector& WorldCoords, FIntPoint& OutAxialCoords);

	/////////UTILITIES

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Utils")
		static void SnapToGrid(const FVector& WorldCoords, FVector& OutSnappedCoords, bool bKeepZ = true);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Utils")
		static int32 LengthOf(const FIntPoint Point);

	//////DIRECTIONS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library | Directions")
		static FIntPoint AxialDirection(EAxialDirection Direction);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library | Directions")
		static FIntPoint Neighbor(const FIntPoint& Hex, EAxialDirection Direction);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library")
		static void Neighbors(const FIntPoint& Hex, TArray<FIntPoint>& OutNeighbors, int32 Radius = 1);
};
