// Fill out your copyright notice in the Description page of Project Settings.

#include "GASCharacter.h"
#include "UnrealNetwork.h"
#include "GloomGameMode.h"
#include "GloomPlayerController.h"
#include "GloomGameState.h"
#include "AbilitySystemComponent.h"
#include "ScenarioInterface.h"

// Sets default values
AGASCharacter::AGASCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

void AGASCharacter::SetupAbilitySystem()
{
	AbilityList.SetNum(AbilitySet->GetNumAbilities());
	AbilitySet->GiveGloomAbilities(AbilitySystemComponent, AbilityList);
}

void AGASCharacter::SetLeadingAction(int32 ActionID)
{
	if (Role == ROLE_Authority)
	{
		LeadingActionIndex = ActionID;
		AGloomPlayerController* PC = Cast<AGloomPlayerController>(GetController());
		if (PC)
		{
			PC->SetInitiative(AbilityList[LeadingActionIndex].Priority);
		}
	}
	else
	{
		Server_SetLeadingAction(ActionID);
	}
}

bool AGASCharacter::Server_SetLeadingAction_Validate(int32 ActionID)
{
	return AbilityList.IsValidIndex(ActionID);
}

void AGASCharacter::Server_SetLeadingAction_Implementation(int32 ActionID)
{
	SetLeadingAction(ActionID);
}


void AGASCharacter::SetSecondaryAction(int32 ActionID)
{
	if (Role == ROLE_Authority)
	{
		SecondaryActionIndex = ActionID;
	}
	else
	{
		Server_SetSecondaryAction(ActionID);
	}
}

bool AGASCharacter::Server_SetSecondaryAction_Validate(int32 ActionID)
{
	return AbilityList.IsValidIndex(ActionID);
}

void AGASCharacter::Server_SetSecondaryAction_Implementation(int32 ActionID)
{
	SetSecondaryAction(ActionID);
}

void AGASCharacter::SetAbilityLocation(int32 AbilityID, EAbilityLocation Location)
{
	if (Role == ROLE_Authority)
	{
		AbilityList[AbilityID].Location = Location;
	}
	else
	{
		Server_SetAbilityLocation(AbilityID, Location);
	}
}

bool AGASCharacter::Server_SetAbilityLocation_Validate(int32 AbilityID, EAbilityLocation Location)
{
	return AbilityList.IsValidIndex(AbilityID);
}

void AGASCharacter::Server_SetAbilityLocation_Implementation(int32 AbilityID, EAbilityLocation Location)
{
	SetAbilityLocation(AbilityID, Location);
}

void AGASCharacter::TryActivateAbility(int32 AbilityID, bool bIsTop)
{
	if(bIsTop)
		GetAbilitySystemComponent()->TryActivateAbility(AbilityList[AbilityID].TopInfo.Handle, true);
	else
		GetAbilitySystemComponent()->TryActivateAbility(AbilityList[AbilityID].BottomInfo.Handle, true);
}



int32 AGASCharacter::GetActionRange(int32 ActionID, bool bIsTop) const
{
	return bIsTop ? AbilityList[ActionID].TopInfo.Range : AbilityList[ActionID].BottomInfo.Range;
}

void AGASCharacter::AddTag(const FGameplayTag& InTag)
{
	if (Role == ROLE_Authority)
	{
		GameplayTags.AddTag(InTag);
	}
	else
	{
		ServerAddTag(InTag);
	}
}

bool AGASCharacter::ServerAddTag_Validate(const FGameplayTag& InTag)
{
	return true;
}

void AGASCharacter::ServerAddTag_Implementation(const FGameplayTag& InTag)
{
	AddTag(InTag);
}

void AGASCharacter::RemoveTag(const FGameplayTag& TagToRemove)
{
	if (Role == ROLE_Authority)
	{
		GameplayTags.RemoveTag(TagToRemove);
	}
	else
	{
		ServerRemoveTag(TagToRemove);
	}
}

void AGASCharacter::EndTurn()
{
	if (Role == ROLE_Authority)
	{
		IScenarioInterface* SI = Cast<IScenarioInterface>(GetController());
		if (SI)
		{
			SI->Execute_EndTurn(GetController());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Controller does not implement scenario interface!"));
		}
	}
	else
	{
		Server_EndTurn();
	}
}

bool AGASCharacter::Server_EndTurn_Validate()
{
	return true;
}

void AGASCharacter::Server_EndTurn_Implementation()
{
	EndTurn();
}

bool AGASCharacter::ServerRemoveTag_Validate(const FGameplayTag& TagToRemove)
{
	return true;
}

void AGASCharacter::ServerRemoveTag_Implementation(const FGameplayTag& TagToRemove)
{
	RemoveTag(TagToRemove);
}

void AGASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGASCharacter, AbilityList);
	DOREPLIFETIME(AGASCharacter, GameplayTags);
	DOREPLIFETIME(AGASCharacter, LeadingActionIndex);
	DOREPLIFETIME(AGASCharacter, SecondaryActionIndex);
}



