// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBTService.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacterController.h"


void UAIBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	MyBlackboardComponent = OwnerComp.GetBlackboardComponent();

	if(IsValid(MyBlackboardComponent))
		Update(OwnerComp);
}

void UAIBTService::ReceiveTickAI(AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds)
{
	ReceiveSearchStartAI(OwnerController, ControlledPawn);

}

void UAIBTService::ReceiveSearchStartAI(AAIController* OwnerController, APawn* ControlledPawn)
{
}

void UAIBTService::OnStaminaValueChanged(float NewValue, float MaxValue)
{
}

void UAIBTService::Update(UBehaviorTreeComponent& OwnerComp)
{
	UpdateBehavior(OwnerComp);

}

void UAIBTService::UpdateBehavior(UBehaviorTreeComponent& OwnerComp)
{
	float DistanceToTarget = 0;
	float DotProductToTarget = 0;
	AAICharacter* ControlledCharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AAICharacterController* AOwnerController = Cast<AAICharacterController>((OwnerComp.GetAIOwner()));
	AActor* Target = Cast<AActor>(MyBlackboardComponent->GetValueAsObject("EnemyActor"));
	if (!IsValid(ControlledCharacter))return;
	if (!IsValid(AOwnerController))return;

	if (IsValid(Target))
	{

		DistanceToTarget = ControlledCharacter->GetDistanceTo(Target);
		//if (!MyBlackboardComponent->GetValueAsBool("ActivateSupport")) {
		//	MyBlackboardComponent->SetValueAsBool("MeleeAttackRange", DistanceToTarget <= AttackBehaviorRange);
		//}
		//else {
		//	MyBlackboardComponent->SetValueAsBool("MeleeAttackRange", false);
		//}
		MyBlackboardComponent->SetValueAsBool("MeleeAttackRange", DistanceToTarget <= MeleeAttackBehaviorRange);
		MyBlackboardComponent->SetValueAsBool("RangedAttackRange", DistanceToTarget <= RangedAttackBehaviorRange);
		if (DistanceToTarget <= MeleeAttackBehaviorRange)
		{
			if (DistanceToTarget > EnemyFollowRange
				|| CheckMaxCombo <= AOwnerController->AttackComboCount)
			{
				MyBlackboardComponent->SetValueAsBool("RestAttack", true);
			}
			if (AOwnerController->AttackComboCount == 0
				|| CheckMaxCombo <= AOwnerController->AttackComboCount)
			{
				MyBlackboardComponent->SetValueAsBool("SpAttack", UKismetMathLibrary::RandomBoolWithWeight(0.3f));

			}
		}
	}
	else
	{
		//Target = AOwnerController->SetTargetActor(EnemyFollowRange);
		//if (IsValid(Target))
		//	MyBlackboardComponent->SetValueAsObject("EnemyActor", Target);

		float distance = EnemyFollowRange;
		for (AActor* checkActor : AOwnerController->CheckedEnemyActors)
		{
			if (IsValid(checkActor))
			{
				if (distance > UKismetMathLibrary::Vector_Distance(ControlledCharacter->GetActorLocation(), checkActor->GetActorLocation()))
				{
					distance = UKismetMathLibrary::Vector_Distance(ControlledCharacter->GetActorLocation(), checkActor->GetActorLocation());
					Target = checkActor;
				}

			}
		}
		if (IsValid(Target))
				MyBlackboardComponent->SetValueAsObject("EnemyActor", Target);

		MyBlackboardComponent->SetValueAsBool("MeleeAttackRange", false);		
		MyBlackboardComponent->SetValueAsBool("RangedAttackRange", false);


	}
	//DistanceToTarget = ControlledCharacter->GetDistanceTo(Target);
	//if (DistanceToTarget > EnemyFollowRange)
	//{
	//	float distance = EnemyFollowRange;
	//	for (AActor* checkActor : AOwnerController->CheckedEnemyActors)
	//	{
	//		if (IsValid(checkActor))
	//		{
	//			if (distance > UKismetMathLibrary::Vector_Distance(ControlledCharacter->GetActorLocation(), checkActor->GetActorLocation()))
	//			{
	//				distance = UKismetMathLibrary::Vector_Distance(ControlledCharacter->GetActorLocation(), checkActor->GetActorLocation());
	//				Target = checkActor;
	//			}

	//		}
	//	}
	//	if (IsValid(Target))
	//		MyBlackboardComponent->SetValueAsObject("EnemyActor", Target);
	//}

	UpdateActivities();

}

void UAIBTService::UpdateActivities()
{
}

void UAIBTService::SetBehavior(EC_AIBehavior Behavior)
{
	MyBlackboardComponent->SetValueAsEnum("CBehavior", (uint8)Behavior);

}

void UAIBTService::Stamina()
{
	IsOutOfStamina = false;
}
