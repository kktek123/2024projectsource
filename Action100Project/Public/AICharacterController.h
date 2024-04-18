// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacterController.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API AAICharacterController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<AActor*> CheckedEnemyActors;

	UFUNCTION(BlueprintCallable)
	void AddCheckedEnemyActors(AActor* addactor);

	UFUNCTION(BlueprintCallable)
		AActor* SetTargetActor(float dist);


	UPROPERTY(EditAnywhere, BlueprintReadwrite)
	int32 AttackComboCount;
};
