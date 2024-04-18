// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EpuipFunctionInterface.h"
#include "AICharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEndAnimDelegate)
DECLARE_MULTICAST_DELEGATE(FOnAnimChainAnimDelegate)

UCLASS()
class ACTION100PROJECT_API AAICharacter : public ACharacter, public IEpuipFunctionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	FOnEndAnimDelegate OnEndAnim;
	FOnAnimChainAnimDelegate OnAnimChain;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Attack();

	UFUNCTION(BlueprintCallable)
		void Abillity1();

	UFUNCTION(BlueprintCallable)
		void Reset();

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		FVector TargetLocation;

	/** 플레이어의 최대 체력. 체력의 최댓값입니다. 이 값은 스폰 시 시작되는 캐릭터의 체력 값입니다.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxStunValue;

	/** 플레이어의 현재 체력. 0이 되면 죽은 것으로 간주됩니다.*/
	UPROPERTY()
		float CurrentStunValue;


	/** 프로퍼티 리플리케이션 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** 대미지를 받는 이벤트. APawn에서 오버라이드됩니다.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* HitReactionMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh_R;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* WeaponCollision_R;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpAttack, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SpAttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = SpAttack, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* SpParticleSystemComponent;


	UPROPERTY()
		FName AnimNextSectionName;


	UPROPERTY(ReplicatedUsing = CollisionOverlap,EditAnywhere, BlueprintReadwrite)
		class USphereComponent* AbillityTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DamageSystem, meta = (AllowPrivateAccess = "true"))
		class UDamageSystem* DamageSystemComponent;


public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		bool bIsAttacking;
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadwrite)
		bool bIsStun;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		bool bIsDamaged;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		bool bIsDead;


	UFUNCTION(Server, Reliable)
		void ActivateAbillity();
	UFUNCTION(NetMultiCast, Reliable)
		void ActivateAbillityMulticast();
	UFUNCTION(Server, Reliable)
		void DeactivateAbillity();
	UFUNCTION(Category = "Trigger")
		void OnAbillityTrigger(AActor* OtherActor,float Radius);
	UFUNCTION(Category = "Trigger")
		void OnAbillityBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Category = "Trigger")
		void OnAbillityEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void CollisionOverlap();
	/** 발사체를 스폰하는 서버 함수*/
	UFUNCTION()
		void HandleAnimtion(UAnimMontage* Montage, FName SectionName);	
	UFUNCTION(Server, Reliable)
		void HandleAnimtionServer(UAnimMontage* Montage, FName SectionName);
	UFUNCTION(NetMulticast, Reliable)
		void HandleAnimtionMulticast(UAnimMontage* Montage, FName SectionName);
	
	UFUNCTION()
		void CheckAnimPlay(UAnimMontage* CurrentAnimMontage);

	UFUNCTION(Server, Reliable)
		virtual void Death();

	UFUNCTION(Server, Reliable)
		void ShotFire();
	
	UFUNCTION()
		void MontageComplete(class UAnimMontage* montage,bool bInterrupted);


	UFUNCTION()
		void StunCoolTime();
	UFUNCTION()
		void StunRecover();



	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Weapon")
		virtual void SetWeaponCollision(ECollisionChannel Channel, ECollisionResponse NewResponse) override;

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Weapon")
		virtual void SetSpAttackCollision(ECollisionChannel Channel, ECollisionResponse NewResponse) override;
	UFUNCTION(BlueprintCallable, Category = "Damage")
		virtual void SetCurrentDamageInfo(FDamageInfo DamageInfo) override;

	UFUNCTION()
		void OnRep_CurrentDamageInfo();

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Weapon")
		virtual void CollisionTakeDamage(AActor* otherActor, FDamageInfo DamageInfo) override;

	UFUNCTION(Client, Reliable, Category = "Damage")
		virtual void OnWeaponOverlapClient(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnTakeDamage(AActor* OtherActor, FDamageInfo DamageInfo);
	virtual void OnApplyDamage(AActor* OtherActor, FDamageInfo DamageInfo);

public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		UParticleSystem* hitParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		USoundBase* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		FDamageInfo CurrentDamageInfo;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		TSubclassOf<class ATestMpProjectile> spawnedProjectile;
};
