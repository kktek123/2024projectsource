// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.h"
#include "EpuipFunctionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEpuipFunctionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTION100PROJECT_API IEpuipFunctionInterface
{
	GENERATED_BODY()

public:
	virtual void SetWeaponCollision(ECollisionChannel Channel, ECollisionResponse NewResponse);
	virtual void SetSpAttackCollision(ECollisionChannel Channel, ECollisionResponse NewResponse);
	virtual void CollisionTakeDamage(AActor* otherActor, FDamageInfo DamageInfo);
	virtual void SetCurrentDamageInfo(FDamageInfo DamageInfo);
};
