// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GloomClassWidget.generated.h"

class AGASCharacter;
/**
 * 
 */
UCLASS()
class SERAPH_API UGloomClassWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gloom")
		TSubclassOf<AGASCharacter> PawnClass;
	
	UFUNCTION(BlueprintCallable, Category = "Widgets|Gloom")
		void SubmitPawnSelection();
	
};
