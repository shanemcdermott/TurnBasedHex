// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graphs/GameNode.h"
#include "HexNode.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class SERAPH_API AHexNode : public AGameNode
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		UMaterialInstanceDynamic* NodeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		FLinearColor NodeColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		float NodeOpacity;

	UFUNCTION(BlueprintCallable, Category = "HexNode|Display")
		void SetColor(const FLinearColor& InColor);

	UFUNCTION(BlueprintCallable, Category = "HexNode|Display")
		void SetOpacity(float InOpacity);
};
