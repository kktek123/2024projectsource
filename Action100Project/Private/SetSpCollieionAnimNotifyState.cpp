// Fill out your copyright notice in the Description page of Project Settings.


#include "SetSpCollieionAnimNotifyState.h"
#include "EpuipFunctionInterface.h"

void USetSpCollieionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	IEpuipFunctionInterface* Interface = Cast<IEpuipFunctionInterface>(MeshComp->GetOwner());
	if (Interface != nullptr) {
		Interface->SetCurrentDamageInfo(CurrentDamageInfo);
		Interface->SetSpAttackCollision(Channel, NewResponse);
	}
}

void USetSpCollieionAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void USetSpCollieionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	IEpuipFunctionInterface* Interface = Cast<IEpuipFunctionInterface>(MeshComp->GetOwner());
	if (Interface != nullptr) {
		Interface->SetSpAttackCollision(Channel, ECR_Ignore);
	}

}
