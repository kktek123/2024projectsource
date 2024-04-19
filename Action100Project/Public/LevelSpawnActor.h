// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSpawnActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossHpBarDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelEndDelegate)
DECLARE_MULTICAST_DELEGATE(FOnLevelStartDelegate)

UCLASS()
class ACTION100PROJECT_API ALevelSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSpawnActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* OverlapTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* FireWallEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* TriggerWall;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadwrite, Category = "SpawnChar")
		TArray<class UParticleSystemComponent*> SpawnLocations;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadwrite, Category = "SpawnChar")
		class UParticleSystemComponent* BossSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "SpawnChar")
		TArray <TSubclassOf<class AAICharacter>> spawnCharClass;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "SpawnChar")
		TSubclassOf<class AAICharacter> spawnBossClass;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "SpawnChar")
		class AEnemyCharacter* spawnBoss;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "SpawnChar")
		TArray <class AAICharacter*> spawnedChars;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "SpawnChar")
		int32 SpawnCount;
	int32 SpawnCounted = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
		void WallActive(bool bActive, ECollisionEnabled::Type type);

	UFUNCTION(Server, Reliable)
		void SpawnCharacters();
	UFUNCTION(Server, Reliable)
		void SpawnAddCharacters(int addIndex);

	UFUNCTION(Category = "Trigger")
		void OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool bisSpawned;

	UPROPERTY(BlueprintAssignable, Category = "UI")
		FOnBossHpBarDelegate OnBossHp;

	FOnLevelEndDelegate OnLevelEnd;
	FOnLevelStartDelegate OnLevelStart;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "SpawnChar")
		bool bBossSpawn;

	UFUNCTION(NetMulticast, Reliable,Category = "Effect")
		void ActiveEffect(int32 index);
	UFUNCTION(NetMulticast, Reliable, Category = "Effect")
		void DeActiveEffect(int32 index);
};
