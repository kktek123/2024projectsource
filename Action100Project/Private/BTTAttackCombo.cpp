// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTAttackCombo.h"
#include "AIController.h"
#include "AICharacter.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacterController.h"


EBTNodeResult::Type UBTTAttackCombo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

	AICharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AICharacter->bIsDamaged)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		bIsFinshed = false;
		AICharacter->CheckAnimPlay(AnimMontage);
		AICharacter->HandleAnimtion(AnimMontage, AICharacter->AnimNextSectionName);
		OwnerComp.GetAIOwner()->SetFocus(
			Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnemyActor")));

		AICharacter->OnAnimChain.AddLambda([&]()
			{
				Cast<AAICharacterController>(OwnerComp.GetAIOwner())->AttackComboCount += 1;
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});

		return EBTNodeResult::InProgress;
	}
}
