// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"

#include "GASTypes.h"
#include "GASMelee.generated.h"

UCLASS(config=Game)
class AGASMelee : public AActor
{
	GENERATED_UCLASS_BODY()

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (ExposeOnSpawn = true))
		float SpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (ExposeOnSpawn = true))
		FGASEffectApplicationContainer OnHitEffectsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollisionComponent;
	
	UPROPERTY(Replicated)
		FVector SpawnOrigin;
};
