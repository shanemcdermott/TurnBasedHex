// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "GASAttributeSet.h"


#include "SeraphCharacter.generated.h"

class UGloomAbilitySet;

UCLASS(config=Game)
class ASeraphCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_UCLASS_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;




	// -------------------------------------------------------------------------------------------
	//			GameplayAbility System Sample
	// -------------------------------------------------------------------------------------------

	UPROPERTY()
		UAbilitySystemComponent* AbilitySystemComponent;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	UPROPERTY()
		UGASAttributeSet*	AttributeSet;


	/** Abilities we will give this character on spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
		UGloomAbilitySet* AbilitySet;
	
	virtual void SetupAbilitySystem();

	virtual float GetMaxSpeed() const
	{
		return AttributeSet->MaxMovementSpeed;
	}
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual float GetMaxHealth() const;

	// -------------------------------------------------------------------------------------------

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;

	virtual void TornOff() override;
	void Die(AController* Killer, AActor* DamageCauser, const FGameplayEffectSpec& KillingEffectSpec, float KillingDamageMagnitude, FVector KillingDamageNormal);
	void PlayDying();

	bool bIsDying;

	FTimerHandle PlayDyingTimerHandle;
	// -------------------------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality")
		FString Name;

	UFUNCTION(BlueprintCallable, Category = "Character|Personality")
		FORCEINLINE FString GetName() const
	{
		return Name;
	}
	// -------------------------------------------------------------------------------------------

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

