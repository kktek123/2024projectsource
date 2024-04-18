// Fill out your copyright notice in the Description page of Project Settings.


#include "SidkickCharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

void ASidkickCharacterController::ActivateSupport_Implementation()
{
	GetBlackboardComponent()->SetValueAsBool("ActivateSupport", true);
	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle,	FTimerDelegate::CreateLambda([&]() {
		GetBlackboardComponent()->SetValueAsBool("ActivateSupport", false);
		}), 2.0f, false);
}
