// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexagonLib.generated.h"

UENUM(BlueprintType)
enum class EHexDirection : uint8
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
	static const FVector2D AxialForwardRight;
	static const FVector2D AxialRight;
	static const FVector2D AxialBackwardRight;
	static const FVector2D AxialBackwardLeft;
	static const FVector2D AxialLeft;
	static const FVector2D AxialForwardLeft;
	static const TArray<FVector2D> AxialDirections;

	static const FVector CubeForwardRight;
	static const FVector CubeRight;
	static const FVector CubeBackwardRight;
	static const FVector CubeBackwardLeft;
	static const FVector CubeLeft;
	static const FVector CubeForwardLeft;
	static const TArray<FVector> CubeDirections;

	///////CONSTANTS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Constants")
	static float Sqrt3Div2();
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Constants")
		static FVector2D GetHexSize();
	
	///////CONVERSIONS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Conversions")
		static void AxialToWorld(int32 X, int32 Y, FVector& OutWorldCoords);

	static FVector AxialToWorld(const FIntPoint& InHex);
	static FVector AxialToWorld(const FVector2D& InHex);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Conversions")
		static void WorldToAxial(const FVector& WorldCoords, FIntPoint& OutAxialCoords);

	static FIntPoint WorldToAxial(const FVector& WorldCoords);

	/////////UTILITIES

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Utils")
		static void SnapToGrid(const FVector& WorldCoords, FVector& OutSnappedCoords, bool bKeepZ = true);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library|Utils")
		static int32 LengthOf(const FIntPoint Point);

	//////DIRECTIONS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library | Directions")
		static FVector2D AxialDirection(EHexDirection Direction);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library | Directions")
		static FVector2D Neighbor(const FVector2D& Hex, EHexDirection Direction);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library | Directions")
		static FVector WorldNeighbor(const FVector& WorldCoord, EHexDirection Direction);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library")
		static void Neighbors(const FVector2D& Hex, TArray<FVector2D>& OutNeighbors, int32 Radius = 1);
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Hexagon Library")
		static void WorldNeighbors(const FVector& WorldCoords, TArray<FVector>& OutNeighbors, int32 Radius = 1);
};
