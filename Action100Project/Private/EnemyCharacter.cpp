// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "../Action100ProjectGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "DamageSystem.h"
#include "Kismet/GameplayStatics.h"


AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::Death()
{
	Super::Death();
	SpawnNextCharacter();
}


void AEnemyCharacter::SpawnNextCharacter_Implementation()
{
	OnCharSpawn.Broadcast(SelfIndex);
}

void AEnemyCharacter::OnTakeDamage_Implementation(AActor* OtherActor, FDamageInfo DamageInfo)
{
	if (GetLocalRole() == ROLE_Authority)
	DamageSystemComponent->TakeDamage(DamageInfo);
}

void AEnemyCharacter::OnApplyDamage_Implementation(AActor* OtherActor, FDamageInfo DamageInfo)
{
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		IEpuipFunctionInterface* Interface = Cast<IEpuipFunctionInterface>(OtherActor);
		if (Interface != nullptr) {
			Interface->CollisionTakeDamage(this,DamageInfo);
		}
	}


}
