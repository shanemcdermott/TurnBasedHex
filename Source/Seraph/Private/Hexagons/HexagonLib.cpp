// Fill out your copyright notice in the Description page of Project Settings.

#include "HexagonLib.h"
#ifndef HEX_HEIGHT
#define HEX_HEIGHT 200.f
#define SQRT3DIV2 0.866f
#endif
const FVector2D UHexagonLib::HexSize = FVector2D(HEX_HEIGHT * SQRT3DIV2, HEX_HEIGHT);
const FVector2D UHexagonLib::HexHalfSize = FVector2D(HexSize.X * 0.5f, HexSize.Y * 0.5f);

//DIRECTIONS
const FVector2D UHexagonLib::AxialRight = FVector2D(1,0);
const FVector2D UHexagonLib::AxialForwardRight = FVector2D(1, 1);
const FVector2D UHexagonLib::AxialForwardLeft = FVector2D(0, -1);
const FVector2D UHexagonLib::AxialLeft = FVector2D(-1, 0);
const FVector2D UHexagonLib::AxialBackwardLeft = FVector2D(-1,1);
const FVector2D UHexagonLib::AxialBackwardRight = FVector2D(0, 1);

const TArray<FVector2D> UHexagonLib::AxialDirections = 
		{
			AxialRight, 
			AxialForwardRight, 
			AxialForwardLeft,
			AxialLeft,
			AxialBackwardLeft,
			AxialBackwardRight
		};

const FVector UHexagonLib::CubeRight = FVector(1,-1, 0);
const FVector UHexagonLib::CubeForwardRight = FVector(1, 0, -1);
const FVector UHexagonLib::CubeForwardLeft = FVector(0, 1, -1);
const FVector UHexagonLib::CubeLeft = FVector(-1, 1, 0);
const FVector UHexagonLib::CubeBackwardLeft = FVector(-1, 0, 1);
const FVector UHexagonLib::CubeBackwardRight = FVector(0, -1, 1);

const TArray<FVector> UHexagonLib::CubeDirections =
{
	CubeRight,
	CubeForwardRight,
	CubeForwardLeft,
	CubeLeft,
	CubeBackwardLeft,
	CubeBackwardRight
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

FVector UHexagonLib::AxialToWorld(const FIntPoint& InHex)
{
	FVector WorldCoords;

	WorldCoords.X = HexSize.X * InHex.X;
	//OutWorldCoords.Y = HexSize.Y * (Y + (X - (X & 1)) / 2);

	if (InHex.X % 2 == 0)
	{
		WorldCoords.Y = HexSize.Y * InHex.Y;
	}
	else
	{
		WorldCoords.Y = HexSize.Y * InHex.Y - HexHalfSize.Y;
	}

	return WorldCoords;
}

FVector UHexagonLib::AxialToWorld(const FVector2D& InHex)
{
	FVector WorldCoords;

	WorldCoords.X = HexSize.X * InHex.X;
	//OutWorldCoords.Y = HexSize.Y * (Y + (X - (X & 1)) / 2);

	if ((int)InHex.X % 2 == 0)
	{
		WorldCoords.Y = HexSize.Y * InHex.Y;
	}
	else
	{
		WorldCoords.Y = HexSize.Y * InHex.Y - HexHalfSize.Y;
	}

	return WorldCoords;
}

void UHexagonLib::WorldToAxial(const FVector& WorldCoords, FIntPoint& OutAxialCoords)
{
	OutAxialCoords.X = FMath::RoundToInt(WorldCoords.X / HexSize.X);
	OutAxialCoords.Y = FMath::RoundToInt(WorldCoords.Y / HexSize.Y);
}

FIntPoint UHexagonLib::WorldToAxial(const FVector& WorldCoords)
{
	return FIntPoint(FMath::RoundToInt(WorldCoords.X / HexSize.X), FMath::RoundToInt(WorldCoords.Y / HexSize.Y));
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

FVector2D UHexagonLib::AxialDirection(EHexDirection Direction)
{
	return AxialDirections[(int32)Direction];
}

FVector2D UHexagonLib::Neighbor(const FVector2D& Hex, EHexDirection Direction)
{
	FVector2D Dir = AxialDirection(Direction);
	return FVector2D(Hex.X + Dir.X, Hex.Y + Dir.Y);
}

void UHexagonLib::Neighbors(const FVector2D& Hex, TArray<FVector2D>& OutNeighbors, int32 Radius /*= 1*/)
{
	OutNeighbors.SetNum(AxialDirections.Num());
	for (int32 i = 0; i < OutNeighbors.Num(); i++)
	{
		OutNeighbors[i] = Neighbor(Hex, EHexDirection(i));
	}
}

FVector UHexagonLib::WorldNeighbor(const FVector& WorldCoord, EHexDirection Direction)
{
	return AxialToWorld(Neighbor(WorldToAxial(WorldCoord), Direction));
}

void UHexagonLib::WorldNeighbors(const FVector& WorldCoords, TArray<FVector>& OutNeighbors, int32 Radius /*= 1*/)
{
	OutNeighbors.SetNum(AxialDirections.Num());
	for (int32 i = 0; i < OutNeighbors.Num(); i++)
	{
		OutNeighbors[i] = WorldNeighbor(WorldCoords, EHexDirection(i));
	}
}

