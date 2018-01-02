// Fill out your copyright notice in the Description page of Project Settings.

#include "GASMelee.h"
#include "UnrealNetwork.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AGASMelee::AGASMelee(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBlockComp"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->bAbsoluteScale = true;
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->SetHiddenInGame(false);
	//CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	RootComponent = CollisionComponent;

	SpawnOffset = 100.f;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGASMelee::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		if (Instigator)
		{
			SetActorTransform(Instigator->GetActorTransform(), false);
		}
		SpawnOrigin = GetActorLocation() + GetActorForwardVector() * SpawnOffset;
	}

	SetActorLocation(SpawnOrigin);
}

void AGASMelee::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGASMelee, SpawnOrigin, COND_InitialOnly);
}


