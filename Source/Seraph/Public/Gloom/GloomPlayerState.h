// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GloomPlayerState.generated.h"

class AGloomPlayerController;
/**
 * 
 */
UCLASS()
class SERAPH_API AGloomPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Scenario|Pawn")
		AGloomPlayerController* Controller;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Scenario")
		bool bIsReadyToStart;
	UFUNCTION(BlueprintCallable, Category = "Scenario|Player")
		FORCEINLINE bool IsReadyToStart() const
	{
		return bIsReadyToStart;
	};

	UFUNCTION(BlueprintCallable, Category = "Scenario|Player")
		void SetReadyToStart(bool bIsReady);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetReadyToStart(bool bIsReady);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
