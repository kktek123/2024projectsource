// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ShotProjecttileAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API UShotProjecttileAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animationconst, const FAnimNotifyEventReference& EventReference) override;


};
