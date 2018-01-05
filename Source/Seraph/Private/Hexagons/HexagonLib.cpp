// Fill out your copyright notice in the Description page of Project Settings.

#include "HexagonLib.h"
#ifndef HEX_HEIGHT
#define HEX_HEIGHT 200.f
#define SQRT3DIV2 0.866f
#endif
const FVector2D UHexagonLib::HexSize = FVector2D(HEX_HEIGHT * SQRT3DIV2, HEX_HEIGHT);
const FVector2D UHexagonLib::HexHalfSize = FVector2D(HexSize.X * 0.5f, HexSize.Y * 0.5f);

//DIRECTIONS
const FIntPoint UHexagonLib::AxialForwardRight = FIntPoint(1,-1);
const FIntPoint UHexagonLib::AxialRight = FIntPoint(1,0);
const FIntPoint UHexagonLib::AxialBackwardRight = FIntPoint(0,1);
const FIntPoint UHexagonLib::AxialBackwardLeft = FIntPoint(-1,1);
const FIntPoint UHexagonLib::AxialLeft = FIntPoint(-1,0);
const FIntPoint UHexagonLib::AxialForwardLeft = FIntPoint(0,-1);
const TArray<FIntPoint> UHexagonLib::AxialDirections = 
		{
			AxialRight, 
			AxialForwardRight, 
			AxialForwardLeft,
			AxialLeft,
			AxialBackwardLeft,
			AxialBackwardRight
		};

float UHexagonLib::Sqrt3Div2()
{
	return SQRT3DIV2;
}

FVector2D UHexagonLib::GetHexSize()
{
	return HexSize;
}

void UHexagonLib::AxialToWorld(int32 X, int32 Y, FVector& OutWorldCoords)
{
	OutWorldCoords.X = HexSize.X * X;
	//OutWorldCoords.Y = HexSize.Y * (Y + (X - (X & 1)) / 2);
	
	if (X % 2 == 0)
	{
		OutWorldCoords.Y = HexSize.Y * Y;
	}
	else
	{
		OutWorldCoords.Y = HexSize.Y * Y - HexHalfSize.Y;
	}
	
}

void UHexagonLib::WorldToAxial(const FVector& WorldCoords, FIntPoint& OutAxialCoords)
{
	OutAxialCoords.X = FMath::RoundToInt(WorldCoords.X / HexSize.X);
	OutAxialCoords.Y = FMath::RoundToInt(WorldCoords.Y / HexSize.Y);
}

void UHexagonLib::SnapToGrid(const FVector& WorldCoords, FVector& OutSnappedCoords, bool bKeepZ /*= true*/)
{
	OutSnappedCoords.Z = bKeepZ ? WorldCoords.Z : 0.f;
	AxialToWorld(FMath::RoundToInt(WorldCoords.X / HexSize.X), FMath::RoundToInt(WorldCoords.Y / HexSize.Y), OutSnappedCoords);
}

int32 UHexagonLib::LengthOf(const FIntPoint Point)
{
	return Point.X * Point.Y;
}

FIntPoint UHexagonLib::AxialDirection(EAxialDirection Direction)
{
	return AxialDirections[(int32)Direction];
}

FIntPoint UHexagonLib::Neighbor(const FIntPoint& Hex, EAxialDirection Direction)
{
	FIntPoint Dir = AxialDirection(Direction);
	return FIntPoint(Hex.X + Dir.X, Hex.Y + Dir.Y);
}

void UHexagonLib::Neighbors(const FIntPoint& Hex, TArray<FIntPoint>& OutNeighbors, int32 Radius /*= 1*/)
{
	OutNeighbors.SetNum(AxialDirections.Num());
	for (int i = 0; i < OutNeighbors.Num(); i++)
	{
		OutNeighbors[i] = Neighbor(Hex, EAxialDirection(i));
	}
}

