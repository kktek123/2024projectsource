// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UDamageSystem::UDamageSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Health = MaxHealth;
	bIsDead = false;

	// ...
}


// Called when the game starts
void UDamageSystem::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Stun = 0;
	Air = 0;
	bIsDead = false;
	// ...
	
}


// Called every frame
void UDamageSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UDamageSystem::Heal(float amount)
{
	if (bIsDead)
	{
		Health = UKismetMathLibrary::Clamp(amount + Health, 0, MaxHealth);
		return Health;
	}
	return 0.f;
}

float UDamageSystem::TakeDamage(FDamageInfo DamageInfo)
{
	float ResultDamage = GetResultDamage(DamageInfo.Amount, DamageInfo.bShouldDamageIncincible, DamageInfo.bCanBeBlocked);
	//Health -= ResultDamage;
	SetCurrentHealth(ResultDamage, DamageInfo.Stun);
	if(Stun >= MaxStun)
		Stun += DamageInfo.Stun;
	return ResultDamage;
}

float UDamageSystem::GetResultDamage(float DamageValue, bool bInvincible, bool bCanBlock)
{
	float ResultValue = DamageValue;
	if (!bIsDead && (!bIsInvincible || bInvincible))
	{
		if (bIsBlocking && bCanBlock)
		{
			ResultValue -= ResultValue * BlockValue;
		}
		return ResultValue;
	}
	return 0.f;
}



void UDamageSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDamageSystem, Health);
	DOREPLIFETIME(UDamageSystem, MaxHealth);
	DOREPLIFETIME(UDamageSystem, Stun);
	DOREPLIFETIME(UDamageSystem, MaxStun);

}

void UDamageSystem::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void UDamageSystem::OnHealthUpdate()
{
		//서버 전용 함수 기능
		if (GetOwner()->GetLocalRole() == ROLE_Authority)
		{
			if (Health <= 0) {
				bIsDead = true;
				OnDeath.Broadcast();

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
					"Dead"
				);
			}
			if (Stun >= MaxStun) 
			{
				OnStun.Broadcast();
				//OnStun.Clear();
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
				FString::Printf(TEXT("Health %f"), Health / MaxHealth));
		}

}

void UDamageSystem::SetCurrentHealth(float healthValue, float stunValue)
{
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		float damageApplied = Health - healthValue;
		float stunApplied = Stun + stunValue;

		Health = FMath::Clamp(damageApplied, 0.f, MaxHealth);
		Stun = FMath::Clamp(stunApplied, 0.f, MaxStun);
		OnHealthUpdate();
	}
}
