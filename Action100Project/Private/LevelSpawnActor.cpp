// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSpawnActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "EnemyCharacter.h"
#include "../Action100ProjectGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALevelSpawnActor::ALevelSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	OverlapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	OverlapTrigger->SetupAttachment(RootComponent);

	FireWallEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Wall"));
	FireWallEffect->SetupAttachment(RootComponent);

	TriggerWall = CreateDefaultSubobject<UBoxComponent>(TEXT("WallCollision"));
	TriggerWall->SetupAttachment(FireWallEffect);


}

// Called when the game starts or when spawned
void ALevelSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	OverlapTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALevelSpawnActor::OnTrigger);
}

// Called every frame
void ALevelSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSpawnActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALevelSpawnActor, BossSpawnLocation);
	DOREPLIFETIME(ALevelSpawnActor, SpawnLocations);

}

void ALevelSpawnActor::SpawnCharacters_Implementation()
{
	AAction100ProjectGameMode* GameMode = Cast<AAction100ProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (bBossSpawn)
	{
		spawnBoss = Cast<AEnemyCharacter>(GetWorld()->SpawnActor<AAICharacter>(
			spawnBossClass, BossSpawnLocation->GetComponentLocation(),
			BossSpawnLocation->GetRelativeRotation(), FActorSpawnParameters()));
		GameMode->Enemys.Add(spawnBoss);
		spawnBoss->OnCharSpawn.AddUObject(this, &ALevelSpawnActor::SpawnAddCharacters);
		OnBossHp.Broadcast();

		BossSpawnLocation->SetActive(true, true);
		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(
			timerHandle,
			FTimerDelegate::CreateLambda([this]() {
				BossSpawnLocation->SetActive(false, false);
				}), 1.0f, false);


	}

	for (UParticleSystemComponent* SpawnLocation : SpawnLocations)
	{
		AEnemyCharacter* spawnedChar = Cast<AEnemyCharacter>(GetWorld()->SpawnActor<AAICharacter>(
			spawnCharClass[SpawnCounted], SpawnLocation->GetComponentLocation(), 
			SpawnLocation->GetRelativeRotation(), FActorSpawnParameters()));
		spawnedChar->SelfIndex = SpawnCounted + 1;
		spawnedChars.Add(spawnedChar);
		spawnedChar->OnCharSpawn.AddUObject(this, &ALevelSpawnActor::SpawnAddCharacters);
		GameMode->Enemys.Add(spawnedChar);
		ActiveEffect(SpawnCounted);


		SpawnCounted++;
	}
	bisSpawned = true;
}

void ALevelSpawnActor::SpawnAddCharacters_Implementation(int addIndex)
{
	AAction100ProjectGameMode* GameMode = Cast<AAction100ProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (SpawnCounted < spawnCharClass.Num())
	{
		SpawnCounted++;
		UParticleSystemComponent* SpawnLocation = SpawnLocations[SpawnCounted % SpawnLocations.Num()];
		AEnemyCharacter* spawnedChar = Cast<AEnemyCharacter>(GetWorld()->SpawnActor<AAICharacter>(
			spawnCharClass[SpawnCounted - 1], SpawnLocation->GetComponentLocation(),
			SpawnLocation->GetRelativeRotation(), FActorSpawnParameters()));
		if (IsValid(spawnedChar))
		{
			spawnedChar->SelfIndex = addIndex;
			spawnedChars[addIndex - 1] = spawnedChar;
			spawnedChar->OnCharSpawn.AddUObject(this, &ALevelSpawnActor::SpawnAddCharacters);
			GameMode->Enemys.Add(spawnedChar);
		}
		ActiveEffect(SpawnCounted % SpawnLocations.Num());
	}
	else
	{
		int RemainedCount = spawnedChars.Num();
		for (AAICharacter* spawnedChar : spawnedChars)
		{
			if (IsValid(spawnedChar))
			{
				if (spawnedChar->DamageSystemComponent->bIsDead)
					--RemainedCount;
			}
			else {
				--RemainedCount;
			}


		}
		if (RemainedCount <= 0)
		{
			if (bBossSpawn)
			{
				if (!IsValid(spawnBoss))
				{
					WallActive(false, ECollisionEnabled::NoCollision);
				}
				else if (spawnBoss->DamageSystemComponent->bIsDead)
				{
					WallActive(false, ECollisionEnabled::NoCollision);
				}
			}
			else
			{
				WallActive(false, ECollisionEnabled::NoCollision);
			}
		}

	}
}

void ALevelSpawnActor::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnCharacters();
	OnLevelStart.Broadcast();
	OverlapTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ALevelSpawnActor::ActiveEffect_Implementation(int32 index)
{
	SpawnLocations[index]->SetActive(true, true);
	FTimerHandle timerHandle;
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ALevelSpawnActor::DeActiveEffect, index);
	GetWorldTimerManager().SetTimer(timerHandle, RespawnDelegate, 1.0f, false);
}

void ALevelSpawnActor::DeActiveEffect_Implementation(int32 index)
{
	SpawnLocations[index]->SetActive(false, false);
}

void ALevelSpawnActor::WallActive_Implementation(bool bActive, ECollisionEnabled::Type type)
{
	FireWallEffect->SetActive(false);
	TriggerWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnLevelEnd.Broadcast();
}

