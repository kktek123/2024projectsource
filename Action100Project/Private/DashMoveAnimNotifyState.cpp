// Fill out your copyright notice in the Description page of Project Settings.


#include "DashMoveAnimNotifyState.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"



void UDashMoveAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (IsValid(Cast<APlayerCharacter>(MeshComp->GetOwner())))
	{
		Cast<APlayerCharacter>(MeshComp->GetOwner())->bIsDashMove = true;
	}
}

void UDashMoveAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
}

void UDashMoveAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (IsValid(Cast<APlayerCharacter>(MeshComp->GetOwner())))
	{
		Cast<APlayerCharacter>(MeshComp->GetOwner())->bIsDashMove = false;
	}
}
