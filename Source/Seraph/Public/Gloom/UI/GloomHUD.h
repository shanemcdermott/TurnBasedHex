// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ScenarioInterface.h"
#include "GloomHUD.generated.h"

/**
 * 
 */
UCLASS()
class SERAPH_API AGloomHUD : public AHUD, public IScenarioInterface
{
	GENERATED_UCLASS_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD|Messages")
		void OnReceiveAnnouncement(const FString& InAnnouncement);

	virtual void BeginTurn_Implementation() override;
	virtual void EndTurn_Implementation() override;

};
