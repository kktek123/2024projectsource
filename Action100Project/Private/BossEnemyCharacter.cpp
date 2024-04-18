// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyCharacter.h"
#include "Components/CapsuleComponent.h"

ABossEnemyCharacter::ABossEnemyCharacter()
{
	WeaponCollision_R->SetupAttachment(GetMesh(), "HammerCenter");

}
