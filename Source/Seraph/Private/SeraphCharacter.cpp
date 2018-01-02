// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SeraphCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "GameplayAbilitySet.h"

//////////////////////////////////////////////////////////////////////////
// ASeraphCharacter

ASeraphCharacter::ASeraphCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// -------------------------------------------------------------------------------------------
	//			GameplayAbility System Sample
	// -------------------------------------------------------------------------------------------

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>(TEXT("AttributeSet"));

	// -------------------------------------------------------------------------------------------

	bIsDying = false;

}

void ASeraphCharacter::BeginPlay()
{
	Super::BeginPlay();
	// -------------------------------------------------------------------------------------------
	//			GameplayAbility System Sample
	// -------------------------------------------------------------------------------------------

	if (Role == ROLE_Authority && AbilitySet != nullptr)
	{
		AbilitySet->GiveAbilities(AbilitySystemComponent);
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		UE_LOG(LogTemp, Display, TEXT("%s BeginPlay: %s"), *GetFullName(), *GetNameSafe(AbilitySystemComponent->AbilityActorInfo->PlayerController.Get()));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASeraphCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASeraphCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASeraphCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASeraphCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASeraphCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASeraphCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASeraphCharacter::TouchStopped);

	// -------------------------------------------------------------------------------------------
	//			GameplayAbility System Sample
	// -------------------------------------------------------------------------------------------

	AbilitySystemComponent->BindToInputComponent(PlayerInputComponent);

	FGameplayAbiliyInputBinds AbilityBinds(FString(), TEXT("AbilityCancel"), TEXT("EGameplayAbilityInputBinds"), (int32)EGameplayAbilityInputBinds::Ability1);
	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, AbilityBinds);

	// -------------------------------------------------------------------------------------------

}

void ASeraphCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASeraphCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASeraphCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASeraphCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASeraphCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASeraphCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


// -------------------------------------------------------------------------------------------
//			GameplayAbility System Sample
// -------------------------------------------------------------------------------------------

float ASeraphCharacter::GetHealth() const
{
	return AttributeSet->Health;
}

float ASeraphCharacter::GetMaxHealth() const
{
	return AttributeSet->MaxHealth;
}


void ASeraphCharacter::TornOff()
{
	Super::TornOff();

	if (!bIsDying && !IsPendingKill())
	{
		PlayDying();
	}
}

void ASeraphCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		UE_LOG(LogTemp, Display, TEXT("PossessedBy (%s): %s"), *GetNameSafe(NewController), *GetNameSafe(AbilitySystemComponent->AbilityActorInfo->PlayerController.Get()));
	}
}

void ASeraphCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our PlayerController changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();

		UE_LOG(LogTemp, Display, TEXT("%s OnRep_Controller (%s): %s"), *GetFullName(), *GetNameSafe(Controller), *GetNameSafe(AbilitySystemComponent->AbilityActorInfo->PlayerController.Get()));
	}
}

void ASeraphCharacter::Die(AController* Killer, AActor* DamageCauser, const FGameplayEffectSpec& KillingEffectSpec, float KillingDamageMagnitude, FVector KillingDamageNormal)
{
	// start the ragdoll and cancel abilities after a very brief delay.
	GetWorldTimerManager().SetTimer(PlayDyingTimerHandle, this, &ASeraphCharacter::PlayDying, 0.1f, false);

	/*
	AController* const DyingController = (Controller != NULL) ? Controller : Cast<AController>(GetOwner());
	ASeraphGameMode* const GameMode = GetWorld()->GetAuthGameMode<ASeraphGameMode>();
	if (GameMode)
	{
		//update the game mode
		GameMode->KilledBy(Killer, DamageCauser, DyingController, this, KillingEffectSpec);
	}
	*/
}

void ASeraphCharacter::PlayDying()
{
	if (bIsDying)
	{
		return;
	}

	// CleanUpAbilitySystem();
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->EndGameplayCuesFor(this);


	bIsDying = true;
	bTearOff = true;
	bReplicateMovement = false;

	DetachFromControllerPendingDestroy();

	// set it up to remove this body
	SetLifeSpan(5.0f);

	// disable collision on the collision capsule
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// Disable character movement
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);
	}

	// just go full ragdoll for now
	if (USkeletalMeshComponent* SkelMesh = GetMesh())
	{
		//static FName CollisionProfileName(TEXT("Ragdoll"));
		//PrimaryMesh->SetCollisionProfileName(CollisionProfileName);
		SkelMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkelMesh->SetSimulatePhysics(true);

		SkelMesh->SetAllBodiesSimulatePhysics(true);
		SkelMesh->bBlendPhysics = true;
		SkelMesh->SetAnimInstanceClass(nullptr);

		// give it the impulse
		/*
		FDamageEvent& DamageEvent = LastTakeHitInfo.GetDamageEvent();
		FVector ImpulseDir;
		FHitResult Hit;
		DamageEvent.GetBestHitInfo(this, LastTakeHitInfo.PawnInstigator.Get(), Hit, ImpulseDir);

		if (DamageEvent.DamageTypeClass)
		{
		FVector const Impulse = ImpulseDir * DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>()->DamageImpulse;
		PrimaryMesh->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
		}

		// Ignore projectiles
		PrimaryMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
		*/
	}
}