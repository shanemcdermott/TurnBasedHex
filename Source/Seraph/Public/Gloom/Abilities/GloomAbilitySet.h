// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySet.h"
#include "GloomAbilitySet.generated.h"

UENUM(BlueprintType)
namespace EGloomAbilityInputBinds
{
	enum Type
	{
		Ability1				UMETA(DisplayName = "Ability1 (Top)"),
		Ability2				UMETA(DisplayName = "Ability2 (Top)"),
		Ability3				UMETA(DisplayName = "Ability3 (Top)"),
		Ability4				UMETA(DisplayName = "Ability4 (Top)"),
		Ability5				UMETA(DisplayName = "Ability5 (Top)"),
		Ability6				UMETA(DisplayName = "Ability6 (Top)"),
		Ability7				UMETA(DisplayName = "Ability7 (Top)"),
		Ability8				UMETA(DisplayName = "Ability8 (Top)"),
		Ability9				UMETA(DisplayName = "Ability9 (Top)"),
	};
}

USTRUCT(BlueprintType)
struct FGloomAbilityBindInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, Category = BindInfo)
		TEnumAsByte<EGloomAbilityInputBinds::Type>	TopCommand;

	UPROPERTY(EditAnywhere, Category = BindInfo)
		TSubclassOf<UGameplayAbility>	TopAbilityClass;

	UPROPERTY(EditAnywhere, Category = BindInfo)
		TEnumAsByte<EGloomAbilityInputBinds::Type>	BottomCommand;

	UPROPERTY(EditAnywhere, Category = BindInfo)
		TSubclassOf<UGameplayAbility>	BottomAbilityClass;

};

UENUM(BlueprintType)
enum class EAbilityLocation : uint8
{
	Unknown,
	InHand,
	Selected,
	Active,
	Discard,
	Lost,
	Exile
};


USTRUCT(BlueprintType)
struct FGloomAbilityInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
		EAbilityLocation Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
		uint8 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
		FString CardName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
		FString TopDetails;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
		FString BottomDetails;

	UPROPERTY()
		FGameplayAbilitySpecHandle TopHandle;

	UPROPERTY()
		FGameplayAbilitySpecHandle BottomHandle;

};

/**
 * 
 */
UCLASS(BlueprintType)
class SERAPH_API UGloomAbilitySet : public UDataAsset
{
	
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TArray<FGloomAbilityBindInfo>	Abilities;

	void GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent) const;

	void GiveGloomAbilities(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGloomAbilityInfo>& OutInfo) const;

	int32 GetNumAbilities() const;
	
};
