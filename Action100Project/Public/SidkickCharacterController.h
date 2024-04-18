// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacterController.h"
#include "SidkickCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API ASidkickCharacterController : public AAICharacterController
{
	GENERATED_BODY()
public:
	UFUNCTION(Client, Reliable)
		void ActivateSupport();

};
