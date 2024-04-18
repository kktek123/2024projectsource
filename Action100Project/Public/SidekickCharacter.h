// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "SidekickCharacter.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTakeDamageDelegate, AActor*, FDamageInfo)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnApplyDamageDelegate, AActor*)


UCLASS()
class ACTION100PROJECT_API ASidekickCharacter : public AAICharacter
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintImplementableEvent)
		void ActivateSupport();
	UFUNCTION(Client, Reliable)
		void ActivateSupportClient();
	FOnTakeDamageDelegate OnTakeDamageDelegate;
	FOnApplyDamageDelegate OnApplyDamageDelegate;
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Weapon")
	virtual void OnTakeDamage(AActor* OtherActor, FDamageInfo DamageInfo) override;
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Weapon")
	virtual void OnApplyDamage(AActor* OtherActor, FDamageInfo DamageInfo) override;
	
	UPROPERTY()
	class APlayerCharacter* SelfPlayer;
};
