// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTriggerActor.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ALevelTriggerActor::ALevelTriggerActor()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TriggerMesh->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeshCollision"));
	TriggerBox->SetupAttachment(TriggerMesh);

	FireWallEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Wall"));
	FireWallEffect->SetupAttachment(RootComponent);

	TriggerWall = CreateDefaultSubobject<UBoxComponent>(TEXT("WallCollision"));
	TriggerWall->SetupAttachment(FireWallEffect);

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

}

// Called when the game starts or when spawned
void ALevelTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTriggerActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALevelTriggerActor, CurrentHealth);
	//DOREPLIFETIME(ALevelTriggerActor, FireWallEffect);
	//DOREPLIFETIME(ALevelTriggerActor, TriggerWall);

}

float ALevelTriggerActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageAmount;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}

void ALevelTriggerActor::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

void ALevelTriggerActor::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ALevelTriggerActor::OnRep_Wall()
{
	FireWallEffect->SetActive(false);
	TriggerWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WallActive(bWallActive, ECollisionEnabled::NoCollision);
}

void ALevelTriggerActor::OnHealthUpdate()
{	
	//서버 전용 함수 기능
	//if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "WalDactive");
			bWallActive = false;
			//Destroy();
			WallActive(false, ECollisionEnabled::NoCollision);
		}
	}

}
//
//void ALevelTriggerActor::WallActiveServer_Implementation(bool bActive, ECollisionEnabled::Type type)
//{
//	FireWallEffect->SetActive(false);
//	TriggerWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	//Destroy();
//	WallActive(bActive, type);
//}

void ALevelTriggerActor::WallActive_Implementation(bool bActive, ECollisionEnabled::Type type)
{
	FireWallEffect->SetActive(false);
	TriggerWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Destroy();
}

