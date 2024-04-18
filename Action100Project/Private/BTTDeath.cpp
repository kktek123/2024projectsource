// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTDeath.h"
#include "AIController.h"
#include "AICharacter.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacterController.h"

EBTNodeResult::Type UBTTDeath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAICharacter* AICharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AICharacter->Death();
	return EBTNodeResult::Succeeded;
}
