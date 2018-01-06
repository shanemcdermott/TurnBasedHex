// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomAbilitySet.h"
#include "AbilitySystemComponent.h"

UGloomAbilitySet::UGloomAbilitySet(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UGloomAbilitySet::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent) const
{

	for (const FGloomAbilityBindInfo& BindInfo : Abilities)
	{
		if (BindInfo.TopAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.TopAbilityClass->GetDefaultObject<UGameplayAbility>(), 1, (int32)BindInfo.TopCommand));
		}
		if (BindInfo.BottomAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.BottomAbilityClass->GetDefaultObject<UGameplayAbility>(), 1, (int32)BindInfo.BottomCommand));
		}
	}
}

void UGloomAbilitySet::GiveGloomAbilities(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGloomAbilityCardInfo>& OutInfo) const
{
	OutInfo.SetNum(Abilities.Num());
	int i = 0;
	for (const FGloomAbilityBindInfo& BindInfo : Abilities)
	{
		if (BindInfo.TopAbilityClass)
		{
			OutInfo[i].TopInfo.Handle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.TopAbilityClass->GetDefaultObject<UGameplayAbility>(), 1, (int32)BindInfo.TopCommand));
		}
		if (BindInfo.BottomAbilityClass)
		{
			OutInfo[i].BottomInfo.Handle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.BottomAbilityClass->GetDefaultObject<UGameplayAbility>(), 1, (int32)BindInfo.BottomCommand));
		}
	}
}

int32 UGloomAbilitySet::GetNumAbilities() const
{
	return Abilities.Num();
}