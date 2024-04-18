// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

UENUM(BlueprintType)
enum class E_DamageType : uint8
{
	None UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),
	Explosion UMETA(DisplayName = "Explosion"),
	Environment UMETA(DisplayName = "Environment"),
};

UENUM(BlueprintType)
enum class E_DamageResponse : uint8
{
	None UMETA(DisplayName = "None"),
	HitReaction UMETA(DisplayName = "HitReaction"),
	Stagger UMETA(DisplayName = "Stagger"),
	Stun UMETA(DisplayName = "Stun"),
	KnockBack UMETA(DisplayName = "KnockBack"),
};

USTRUCT(Atomic, BlueprintType)
struct FDamageInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Amount = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		E_DamageType DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		E_DamageResponse DamageResponse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShouldDamageIncincible = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeBlocked = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeParried = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShouldForceInterrupt = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Stun = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Air = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PointDegree = 0;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTION100PROJECT_API IIDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
		float GetCurrentHealth();
		float GetMaxHealth();
		float Heal(float amount);
		float TakeDamage(FDamageInfo DamageInfo);

};
