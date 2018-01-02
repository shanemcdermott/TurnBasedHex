// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SeraphCharacter.h"
#include "GASCharacter.generated.h"




UCLASS()
class SERAPH_API AGASCharacter : public ASeraphCharacter
{
	GENERATED_BODY()

public:
	
	UPROPERTY(ReplicatedUsing = OnRep_Initiative, EditAnywhere, BlueprintReadWrite, Category = "Scenario")
		uint8 Initiative;

	// Sets default values for this character's properties
	AGASCharacter();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Online|Scenario")
		void RegisterForCombat();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Initiative")
		void ServerSetInitiative(uint8 Init);
	void ServerSetInitiative_Implementation(uint8 Init);
	bool ServerSetInitiative_Validate(uint8 Init);

	UFUNCTION()
	void OnRep_Initiative();


	virtual void BeginActionSelection();

	UFUNCTION(Client, Reliable)
	void ClientBeginActionSelection();
	void ClientBeginActionSelection_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Initiative")
		void OnBeginActionSelection();


	UFUNCTION(BlueprintCallable, Category = "Initiative")
		void SubmitActionSelection(uint8 ActionA, uint8 ActionB);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSubmitActionSelection(uint8 ActionA, uint8 ActionB);
	void ServerSubmitActionSelection_Implementation(uint8 ActionA, uint8 ActionB);
	bool ServerSubmitActionSelection_Validate(uint8 ActionA, uint8 ActionB);

	virtual void StartTurn();



	UFUNCTION(BlueprintImplementableEvent, Category = "Turn")
		void OnBeginTurn();

	UFUNCTION(Client, Reliable)
		void ClientStartTurn();
	void ClientStartTurn_Implementation();


	UFUNCTION(BlueprintCallable, Category = "Turn")
		virtual void EndTurn();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndTurn();
	void ServerEndTurn_Implementation();
	bool ServerEndTurn_Validate();



	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
