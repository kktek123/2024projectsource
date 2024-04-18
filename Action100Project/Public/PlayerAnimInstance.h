// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAttack;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAbillity1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAbillity2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAbillity3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bEvade;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSkill1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSkill2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FrontMoveValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Direction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bHit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bStun;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAir;
};
