// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IDamageable.h"
#include "DamageSystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate)
DECLARE_MULTICAST_DELEGATE(FOnStunDelegate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTION100PROJECT_API UDamageSystem : public UActorComponent, public IIDamageable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth,EditAnywhere, BlueprintReadWrite)
		float Health;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, EditAnywhere, BlueprintReadWrite)
		float MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth,EditAnywhere, BlueprintReadWrite)
		float Stun;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, EditAnywhere, BlueprintReadWrite)
		float MaxStun;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth,EditAnywhere, BlueprintReadWrite)
		float Air;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, EditAnywhere, BlueprintReadWrite)
		float MaxAir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsInvincible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsInterruptible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsBlocking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlockValue;
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetCurrentHealth() { return Health; }
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetMaxHealth() { return MaxHealth; }
	UFUNCTION(BlueprintCallable, Category = "Health")
		float Heal(float amount);
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(FDamageInfo DamageInfo);

	float GetResultDamage(float DamageValue, bool bInvincible, bool bCanBlock);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
		void OnRep_CurrentHealth();

	/** ������Ʈ�Ǵ� ü�¿� ����. ���������� ���� ��� ȣ��, Ŭ���̾�Ʈ������ RepNotify�� �����Ͽ� ȣ��*/
	void OnHealthUpdate();

	/** ���� ü�� ����. ���� 0�� MaxHealth ���̷� ���������ϰ� OnHealthUpdate�� ȣ���մϴ�. ���������� ȣ��Ǿ�� �մϴ�.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue,float stunValue);
	FOnDeathDelegate OnDeath;
	FOnStunDelegate OnStun;
};
