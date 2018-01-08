// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ScenarioInterface.h"
#include "Gloom/Interfaces/ScenarioControllerInterface.h"
#include "GloomPlayerController.generated.h"

class AGASCharacter;
class AGloomHUD;
/**
 * 
 */
UCLASS()
class SERAPH_API AGloomPlayerController : public APlayerController, public IScenarioInterface, public IScenarioControllerInterface
{
	GENERATED_UCLASS_BODY()
	
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		AGloomHUD* GloomHUD;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gloom|Online")
		int32 PlayerId;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Scenario")
		bool bIsReadyToStartScenario;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Scenario")
		bool bIsReadyToStartRound;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Scenario|Pawn")
		TSubclassOf<AGASCharacter> PawnClass;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Scenario")
		uint8 Initiative;

	
	virtual uint8 GetInitiativeValue_Implementation() const override;
	virtual FString GetCharacterName_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Initiative")
		void SetInitiative(uint8 Init);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetInitiative(uint8 Init);


	UFUNCTION(BlueprintCallable, Category = "Gloom|Pawn")
		void SetPawnClass(TSubclassOf<AGASCharacter> InPawnClass);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPawnClass(TSubclassOf<AGASCharacter> InPawnClass);

	UFUNCTION(BlueprintNativeEvent)
		void PerformScenarioSetup();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	virtual bool IsReadyToStartScenario() const override;


		virtual void PrepareForRound() override;

	UFUNCTION(BlueprintCallable, Category = "Scenario")
		virtual bool IsReadyToStartRound() const override;

	UFUNCTION(BlueprintNativeEvent)
		void BeginRound();

	void AnnounceTurnStart();
	virtual void BeginTurn_Implementation() override;


	void AnnounceTurnEnd();
	virtual void EndTurn_Implementation() override;

	UFUNCTION(Client, Reliable)
		void Client_EndTurn();

	UFUNCTION(BlueprintNativeEvent)
		void PerformRoundCleanup();

	UFUNCTION(Client, Reliable)
		void Client_PerformRoundCleanup();

	UFUNCTION(BlueprintCallable, Category = "Character")
		AGASCharacter* GetGloomPawn() const;

	UFUNCTION(BlueprintCallable, Category = "Scenario|Player")
		void SetReadyToStartScenario(bool bIsReady);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetReadyToStartScenario(bool bIsReady);

	UFUNCTION(BlueprintCallable, Category = "Scenario|Player")
		void SetReadyToStartRound(bool bIsReady);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetReadyToStartRound(bool bIsReady);

};
