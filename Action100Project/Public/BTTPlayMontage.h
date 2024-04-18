// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTPlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API UBTTPlayMontage : public UBTTaskNode
{
	GENERATED_BODY()
public:
	

	UPROPERTY(BlueprintReadwrite,EditAnywhere)
		class UAnimMontage* AnimMontage;
protected:
	virtual EBTNodeResult::Type ExecuteTask	(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
private:
	class AAICharacter* AICharacter;
	FTimerHandle AnimTimerHandle;
	bool bIsFinshed;

};
