// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioControllerInterface.generated.h"



// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable, MinimalAPI)
class UScenarioControllerInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class SERAPH_API IScenarioControllerInterface 
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Scenario|Initiative")
		uint8 GetInitiativeValue() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Scenario|Characters")
		FString GetCharacterName() const;

};



