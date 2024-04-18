// Fill out your copyright notice in the Description page of Project Settings.


#include "EpuipFunctionInterface.h"

// Add default functionality here for any IEpuipFunctionInterface functions that are not pure virtual.

void IEpuipFunctionInterface::SetWeaponCollision(ECollisionChannel Channel, ECollisionResponse NewResponse)
{
}

void IEpuipFunctionInterface::SetSpAttackCollision(ECollisionChannel Channel, ECollisionResponse NewResponse)
{
}

void IEpuipFunctionInterface::CollisionTakeDamage(AActor* otherActor, FDamageInfo DamageInfo)
{
}

void IEpuipFunctionInterface::SetCurrentDamageInfo(FDamageInfo DamageInfo)
{
}
