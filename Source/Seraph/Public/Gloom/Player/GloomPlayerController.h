// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ScenarioInterface.h"
#include "GloomPlayerController.generated.h"

class AGASCharacter;
class AGloomPlayerState;
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
		bool bIsReadyToStartRound;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Scenario|Pawn")
		TSubclassOf<AGASCharacter> PawnClass;

	UFUNCTION(BlueprintCallable, Category = "Gloom|Pawn")
		void SetPawnClass(TSubclassOf<AGASCharacter> InPawnClass);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPawnClass(TSubclassOf<AGASCharacter> InPawnClass);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	UFUNCTION()
		AGloomPlayerState* GetPlayerState();

	UFUNCTION(BlueprintNativeEvent)
		void PerformScenarioSetup();

	UFUNCTION(BlueprintNativeEvent)
		void PrepareForRound();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
		bool IsReadyToStartRound() const;

	UFUNCTION(BlueprintNativeEvent)
		void BeginRound();

	UFUNCTION(BlueprintNativeEvent)
		void BeginTurn();

	UFUNCTION(BlueprintNativeEvent)
		void EndTurn();

	UFUNCTION(BlueprintNativeEvent)
		void PerformRoundCleanup();

	UFUNCTION(BlueprintCallable, Category = "Gloom|Pawn")
		AGASCharacter* GetGloomPawn();
};
