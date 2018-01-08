// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomTask_TargetHex.h"
#include "Abilities/GameplayAbility.h"

#include "Gloom/Interfaces/TargetsHexNode.h"



void AGloomTask_TargetHex::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();
	if (SourceActor)
	{
		GetHexTarget();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Null Source Actor!"));
	}
}

void AGloomTask_TargetHex::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		bDebug = false;
		GetHexTarget();
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

void AGloomTask_TargetHex::GetHexTarget()
{
	ITargetsHexNode* SourceTargeting = Cast<ITargetsHexNode>(SourceActor);
	if (SourceTargeting)
	{
		
		AActor* TargetActor = SourceTargeting->GetTarget();
		if (TargetActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Target is %s"), *TargetActor->GetName());
			TargetHexInfo.Add(TWeakObjectPtr<AActor>(TargetActor));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Valid Target!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Source Does not implement HexTarget Interface!"));
	}
}

FGameplayAbilityTargetDataHandle AGloomTask_TargetHex::MakeTargetData() const
{
	
	return StartLocation.MakeTargetDataHandleFromActors(TargetHexInfo);
}
