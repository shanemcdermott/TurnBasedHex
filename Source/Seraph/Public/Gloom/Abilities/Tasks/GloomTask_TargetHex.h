// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GloomTask_TargetHex.generated.h"


/**
 * 
 */
UCLASS()
class SERAPH_API AGloomTask_TargetHex : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:

	TArray<TWeakObjectPtr<AActor>> TargetHexInfo;

	virtual void StartTargeting(UGameplayAbility* InAbility) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void GetHexTarget();

protected:

	FGameplayAbilityTargetDataHandle MakeTargetData() const;
};
