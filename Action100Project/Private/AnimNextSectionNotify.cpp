// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNextSectionNotify.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"

void UAnimNextSectionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (IsValid(Cast<APlayerCharacter>(MeshComp->GetOwner())))
	{
		Cast<APlayerCharacter>(MeshComp->GetOwner())->AnimNextSectionName = NextSectionName;
		Cast<APlayerCharacter>(MeshComp->GetOwner())->bIsAttacking = false;
		Cast<APlayerCharacter>(MeshComp->GetOwner())->bIsEvade = false;

	}
	else if(IsValid(Cast<AAICharacter>(MeshComp->GetOwner())))
	{
		Cast<AAICharacter>(MeshComp->GetOwner())->AnimNextSectionName = NextSectionName;
		Cast<AAICharacter>(MeshComp->GetOwner())->bIsAttacking = false;
		Cast<AAICharacter>(MeshComp->GetOwner())->OnAnimChain.Broadcast();
	}
}
