// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTResetBehaviorValue.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacterController.h"

EBTNodeResult::Type UBTTResetBehaviorValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacter* ControlledCharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AAICharacterController* AOwnerController = Cast<AAICharacterController>((OwnerComp.GetAIOwner()));
	UBlackboardComponent* MyBlackboardComponent = OwnerComp.GetBlackboardComponent();

	MyBlackboardComponent->SetValueAsBool("RestAttack", false);
	AOwnerController->AttackComboCount = 0;
	MyBlackboardComponent->SetValueAsBool("Hit", false);

	return EBTNodeResult::Succeeded;
}
