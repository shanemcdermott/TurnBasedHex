// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SeraphCharacter.h"
#include "GASCharacter.generated.h"




UCLASS()
class SERAPH_API AGASCharacter : public ASeraphCharacter
{
	GENERATED_UCLASS_BODY()

public:
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Scenario")
		uint8 Initiative;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer GameplayTags;


	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Initiative")
		void SetInitiative(uint8 Init);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetInitiative(uint8 Init);


	virtual void BeginActionSelection();

	UFUNCTION(Client, Reliable)
	void ClientBeginActionSelection();

	UFUNCTION(BlueprintImplementableEvent, Category = "Initiative")
		void OnBeginActionSelection();


	UFUNCTION(BlueprintCallable, Category = "Initiative")
		void SubmitActionSelection(uint8 ActionA, uint8 ActionB);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSubmitActionSelection(uint8 ActionA, uint8 ActionB);

	virtual void StartTurn();



	UFUNCTION(BlueprintImplementableEvent, Category = "Turn")
		void OnBeginTurn();

	UFUNCTION(Client, Reliable)
		void ClientStartTurn();


	UFUNCTION(BlueprintCallable, Category = "Turn")
		virtual void EndTurn();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndTurn();

	UFUNCTION(BlueprintCallable, Category = "Tags")
		void AddTag(const FGameplayTag& InTag);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddTag(const FGameplayTag& InTag);

	UFUNCTION(BlueprintCallable, Category = "Tags")
		void RemoveTag(const FGameplayTag& TagToRemove);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveTag(const FGameplayTag& TagToRemove);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
