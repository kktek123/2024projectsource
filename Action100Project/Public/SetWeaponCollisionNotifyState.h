// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SetWeaponCollisionNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API USetWeaponCollisionNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)					  override;
public:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<ECollisionChannel> Channel;
	UPROPERTY(EditAnywhere)
		TEnumAsByte<ECollisionResponse> NewResponse;
	UPROPERTY(EditAnywhere)
		FDamageInfo CurrentDamageInfo;

};
