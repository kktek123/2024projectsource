// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RunEQSStrafeAround.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"


void UBTService_RunEQSStrafeAround::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	MyBlackboardComponent = OwnerComp.GetBlackboardComponent();
	ControlledPawn = (OwnerComp.GetAIOwner()->GetPawn());
	Strafe();
}

void UBTService_RunEQSStrafeAround::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
}

void UBTService_RunEQSStrafeAround::Strafe()
{
}

UEnvQuery* UBTService_RunEQSStrafeAround::GetStrafeQuery()
{
	return nullptr;
}
