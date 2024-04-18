// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTAttackCombo.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API UBTTAttackCombo : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		class UAnimMontage* AnimMontage;
	UPROPERTY()
		FName AnimNextSectionName;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetKey;

	class AAICharacter* AICharacter;
	FTimerHandle AnimTimerHandle;
	bool bIsFinshed;

};
