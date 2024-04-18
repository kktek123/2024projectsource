// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNextSectionNotify.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API UAnimNextSectionNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		FName NextSectionName;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animationconst, const FAnimNotifyEventReference& EventReference) override;
};
