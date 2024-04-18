// Fill out your copyright notice in the Description page of Project Settings.


#include "LichEnemyCharacter.h"
#include "Components/CapsuleComponent.h"

ALichEnemyCharacter::ALichEnemyCharacter()
{
	WeaponCollision_R->SetupAttachment(GetMesh(),"Wrist_R");
	//WeaponCollision_L = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision_L"));
	//WeaponCollision_L->SetupAttachment(GetMesh(),"ik_hand_l");
}


