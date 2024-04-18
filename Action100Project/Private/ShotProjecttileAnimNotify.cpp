// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotProjecttileAnimNotify.h"
#include "TestMpProjectile.h"
#include "AICharacter.h"


void UShotProjecttileAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animationconst, const FAnimNotifyEventReference& EventReference)
{
	if(IsValid(Cast<AAICharacter>(MeshComp->GetOwner())))
	Cast<AAICharacter>(MeshComp->GetOwner())->ShotFire();
}