// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTPlayMontage.h"
#include "AIController.h"
#include "AICharacter.h"
#include "TimerManager.h"

EBTNodeResult::Type UBTTPlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AICharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	//if (AICharacter->bIsDamaged)
	//{
	//	return EBTNodeResult::Failed;
	//}
	//else
	//{
	//	
	//}
	bIsFinshed = false;
	AICharacter->CheckAnimPlay(AnimMontage);
	AICharacter->HandleAnimtion(AnimMontage, AICharacter->AnimNextSectionName);
	//		AICharacter->Attack();
	AICharacter->OnEndAnim.AddLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	return EBTNodeResult::InProgress;
}

void UBTTPlayMontage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsFinshed)
	{
	}
}

void UBTTPlayMontage::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
}
