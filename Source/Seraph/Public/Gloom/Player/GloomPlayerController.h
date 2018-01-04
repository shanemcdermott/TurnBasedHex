// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ScenarioInterface.h"
#include "GloomPlayerController.generated.h"

class AGASCharacter;
class AGloomHUD;
/**
 * 
 */
UCLASS()
class SERAPH_API AGloomPlayerController : public APlayerController, public IScenarioInterface
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

	UFUNCTION(BlueprintCallable, Category = "Initiative")
		uint8 GetInitiativeValue() const;

	UFUNCTION(BlueprintCallable, Category = "Initiative")
		void SetInitiative(uint8 Init);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetInitiative(uint8 Init);


	UFUNCTION(BlueprintCallable, Category = "Gloom|Pawn")
		void SetPawnClass(TSubclassOf<AGASCharacter> InPawnClass);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPawnClass(TSubclassOf<AGASCharacter> InPawnClass);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;


	UFUNCTION(BlueprintNativeEvent)
		void PerformScenarioSetup();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
		virtual bool IsReadyToStartScenario() const override;

	UFUNCTION(BlueprintNativeEvent)
		void PrepareForRound();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
		virtual bool IsReadyToStartRound() const override;

	UFUNCTION(BlueprintNativeEvent)
		void BeginRound();

	UFUNCTION(BlueprintNativeEvent)
		void BeginTurn();

	UFUNCTION(Client, Reliable)
		void Client_BeginTurn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Scenario")
		void EndTurn();

	UFUNCTION(Client, Reliable)
		void Client_EndTurn();

	UFUNCTION(BlueprintNativeEvent)
		void PerformRoundCleanup();

	UFUNCTION(BlueprintCallable, Category = "Gloom|Pawn")
		AGASCharacter* GetGloomPawn();

	UFUNCTION(BlueprintCallable, Category = "Scenario|Player")
		void SetReadyToStartScenario(bool bIsReady);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetReadyToStartScenario(bool bIsReady);

	UFUNCTION(BlueprintCallable, Category = "Scenario|Player")
		void SetReadyToStartRound(bool bIsReady);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetReadyToStartRound(bool bIsReady);
};
