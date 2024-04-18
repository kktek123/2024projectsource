// Fill out your copyright notice in the Description page of Project Settings.


#include "SidekickCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "SidkickCharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"

void ASidekickCharacter::OnTakeDamage_Implementation(AActor* OtherActor, FDamageInfo DamageInfo)
{
	OnTakeDamageDelegate.Broadcast(OtherActor, DamageInfo);
}

void ASidekickCharacter::OnApplyDamage_Implementation(AActor* OtherActor, FDamageInfo DamageInfo)
{
	OnApplyDamageDelegate.Broadcast(OtherActor);
}

void ASidekickCharacter::ActivateSupportClient_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ASidkickCharacterController* SidkickController = Cast<ASidkickCharacterController>(GetController());
		if (IsValid(SidkickController))
		{
			SidkickController->ActivateSupport();
		}
	}
}

void ASidekickCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsValid(SelfPlayer))
	{
		Destroy();
	}
}
