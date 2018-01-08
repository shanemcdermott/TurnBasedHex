// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SeraphCharacter.h"
#include "GASTypes.h"
#include "GloomAbilitySet.h"
#include "Gloom/Interfaces/TargetsHexNode.h"
#include "GASCharacter.generated.h"


class AGameNode;

UCLASS()
class SERAPH_API AGASCharacter : public ASeraphCharacter, public ITargetsHexNode
{
	GENERATED_UCLASS_BODY()

public:
	
	/*The leading action for the turn. Used for initiative*/
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
		int32 LeadingActionIndex;

	/*The second action for the turn.*/
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
		int32 SecondaryActionIndex;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TArray<FGloomAbilityCardInfo> AbilityList;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer GameplayTags;

	virtual void SetupAbilitySystem() override;

	//////////////ABILITIES
	UFUNCTION(BlueprintCallable, Category = "Abilities|Round Setup")
		void SetLeadingAction(int32 ActionID);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_SetLeadingAction(int32 ActionID);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Round Setup")
		void SetSecondaryAction(int32 ActionID);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetSecondaryAction(int32 ActionID);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void SetAbilityLocation(int32 AbilityID, EAbilityLocation Location);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetAbilityLocation(int32 AbilityID, EAbilityLocation Location);


	UFUNCTION(BlueprintCallable, Category = "Abilities|Cards")
		void GetAbilityCardInfo(int32 ActionID, FGloomAbilityCardInfo& OutAbilityCardInfo) const;
	////////////////COMBAT
		
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void TryActivateAbility(int32 AbilityID, bool bIsTop);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		int32 GetActionRange(int32 ActionID, bool bIsTop) const;

	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	/////////TAGS
	UFUNCTION(BlueprintCallable, Category = "Tags")
		void AddTag(const FGameplayTag& InTag);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddTag(const FGameplayTag& InTag);

	UFUNCTION(BlueprintCallable, Category = "Tags")
		void RemoveTag(const FGameplayTag& TagToRemove);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveTag(const FGameplayTag& TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "Scenario|Turns")
		void EndTurn();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_EndTurn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//HEX TARGETS

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Targeting")
		AActor* TargetActor;

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	virtual AActor* GetTarget() override;

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	virtual void SetTarget(AActor* NewTarget) override;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetTarget(AActor* NewTarget);
};
