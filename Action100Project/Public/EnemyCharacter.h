// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharSpawnDelegate,int)

UCLASS()
class ACTION100PROJECT_API AEnemyCharacter : public AAICharacter
{
	GENERATED_BODY()
public:
	AEnemyCharacter();
	void Death() override;
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapon")
		void SpawnNextCharacter();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapon")
		virtual void OnTakeDamage(AActor* OtherActor, FDamageInfo DamageInfo) override;
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Weapon")
		virtual void OnApplyDamage(AActor* OtherActor, FDamageInfo DamageInfo) override;

	FOnCharSpawnDelegate OnCharSpawn;
	int SelfIndex;
};
