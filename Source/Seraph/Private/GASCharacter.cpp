// Fill out your copyright notice in the Description page of Project Settings.

#include "GASCharacter.h"
#include "UnrealNetwork.h"
#include "GloomGameMode.h"

// Sets default values
AGASCharacter::AGASCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	Initiative = 99;
}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGASCharacter::SetInitiative(uint8 Init)
{
	if (Role == ROLE_Authority)
	{
		Initiative = Init;
	}
	else
	{
		ServerSetInitiative(Init);
	}
}

bool AGASCharacter::ServerSetInitiative_Validate(uint8 Init)
{
	return true;
}

void AGASCharacter::ServerSetInitiative_Implementation(uint8 Init)
{
	SetInitiative(Init);
}

void AGASCharacter::BeginActionSelection()
{
	ClientBeginActionSelection();
}

void AGASCharacter::ClientBeginActionSelection_Implementation()
{
	OnBeginActionSelection();
}

void AGASCharacter::SubmitActionSelection(uint8 ActionA, uint8 ActionB)
{
	if (Role == ROLE_Authority)
	{
		Initiative = FMath::Min(ActionA, ActionB);
		AGloomGameMode* GM = Cast<AGloomGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->ReceiveActionSelect(this, ActionA, ActionB);
		}
	}
	else
	{
		ServerSubmitActionSelection(ActionA, ActionB);
	}
}

void AGASCharacter::ServerSubmitActionSelection_Implementation(uint8 ActionA, uint8 ActionB)
{
	SubmitActionSelection(ActionA, ActionB);
}

bool AGASCharacter::ServerSubmitActionSelection_Validate(uint8 ActionA, uint8 ActionB)
{
	return true;
}

void AGASCharacter::StartTurn()
{
	UE_LOG(LogTemp, Log, TEXT("%s: Starting my turn!"), *Name);
	ClientStartTurn();
}

void AGASCharacter::ClientStartTurn_Implementation()
{
	OnBeginTurn();
}

void AGASCharacter::ServerEndTurn_Implementation()
{
	EndTurn();
}

bool AGASCharacter::ServerEndTurn_Validate()
{
	return true;
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

bool AGASCharacter::ServerRemoveTag_Validate(const FGameplayTag& TagToRemove)
{
	return true;
}

void AGASCharacter::ServerRemoveTag_Implementation(const FGameplayTag& TagToRemove)
{
	RemoveTag(TagToRemove);
}

void AGASCharacter::EndTurn()
{
	if (Role == ROLE_Authority)
	{
		AGloomGameMode* GM = Cast<AGloomGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->EndTurn();
		}
	}
	else
	{
		ServerEndTurn();
	}
}

void AGASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGASCharacter, Initiative);
	DOREPLIFETIME(AGASCharacter, GameplayTags);
}



