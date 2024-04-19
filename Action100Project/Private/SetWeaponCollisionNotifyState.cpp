// Fill out your copyright notice in the Description page of Project Settings.


#include "SetWeaponCollisionNotifyState.h"
#include "EpuipFunctionInterface.h"

void USetWeaponCollisionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	IEpuipFunctionInterface* Interface = Cast<IEpuipFunctionInterface>(MeshComp->GetOwner());
	if (Interface != nullptr) {
		Interface->SetCurrentDamageInfo(CurrentDamageInfo);
		Interface->SetWeaponCollision(Channel, NewResponse);
	}
}

void USetWeaponCollisionNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void USetWeaponCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	IEpuipFunctionInterface* Interface = Cast<IEpuipFunctionInterface>(MeshComp->GetOwner());
	if (Interface != nullptr) {
		FDamageInfo DamageInfo;
		Interface->SetWeaponCollision(Channel, ECR_Ignore);
		//Interface->SetCurrentDamageInfo(DamageInfo);
	}

}
