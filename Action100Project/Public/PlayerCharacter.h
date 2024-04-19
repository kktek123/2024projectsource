// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "EpuipFunctionInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config = Game)
class ACTION100PROJECT_API APlayerCharacter : public ACharacter ,public IEpuipFunctionInterface
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* EvadeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* Abillity1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* Abillity2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* Abillity3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* Skill1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* Skill2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* Support1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* Support2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ChangeModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh_R;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* WeaponCollision_R;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh_L;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* WeaponCollision_L;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpAttack, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SpAttackCollision;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DamageSystem, meta = (AllowPrivateAccess = "true"))
		class UDamageSystem* DamageSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Destination, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* DestinationArm;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

public:
	UPROPERTY(ReplicatedUsing = BindSidekickfunction,EditAnywhere, BlueprintReadwrite, Category = Sidekick, meta = (AllowPrivateAccess = "true"))
		class ASidekickCharacter* Sidekick1;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Sidekick, meta = (AllowPrivateAccess = "true"))
		class ASidekickCharacter* Sidekick2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnChar")
		TSubclassOf<class ASidekickCharacter> spawnChar;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaSecond);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** 프로퍼티 리플리케이션 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/** 대미지를 받는 이벤트. APawn에서 오버라이드됩니다.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	/*서버내에서 애니메이션 수행*/
	UFUNCTION(Server, Reliable)
		void HandleAnimtion(UAnimMontage* Montage, FName SectionName);
	/*전 클라에 애니메이션 수행*/
	UFUNCTION(NetMulticast, Reliable)
		void HandleAnimtionMulticast(UAnimMontage* Montage,FName SectionName);
	/*애니메이션 완료시점 콜백*/
	UFUNCTION()
		void MontageComplete(class UAnimMontage* montage, bool bInterrupted);
	/*애니메이션이 실행중인지 확인*/
	UFUNCTION()
		void CheckAnimPlay(UAnimMontage* CurrentAnimMontage);
	/*애니메이션의 다음섹션진행*/
	UFUNCTION()
		void OnRep_SetSectionNameNone();

	/*스턴중인 적을 타겟팅 */
	UFUNCTION(Server, Reliable)
		void CheckTarget();
	/*스턴중인 적을 타겟팅 */
	UFUNCTION(Server, Reliable)
		void CheckTargetAbillity(float TriggerRadius);
	/*Abillity 활성화 */
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void OnRep_SetIsCanAbillity(bool b);
	/*타겟팅 된 엑터*/
	UPROPERTY(Replicated, BlueprintReadwrite, EditAnywhere)
		AActor* TargetActor;
	/*떨어진 타겟에게 접근*/
	UFUNCTION(Server, Reliable)
		void TargetDashMove(FVector TargetLocation, float Delta, float speed);
	/*타겟에게 로테이트 고정*/
	UFUNCTION(Server, Reliable)
		void LockRotateTarget(FVector TargetLocation);

public:
	bool bIsLookValueZero;
	bool bIsDashMove;
	UPROPERTY(Replicated, EditAnywhere, Blueprintreadwrite)
		bool bIsCanAbillity;
	UPROPERTY(EditAnywhere, Blueprintreadwrite)
		float ActiveTriggerdistance;
	bool bLockTarget;


protected:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Attack();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void AttackEnd();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Evade();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void EvadeEnd();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Skill1();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Skill1End();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Skill2();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Skill2End();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Abillity1();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Abillity1End();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Abillity2();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Abillity2End();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Abillity3();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Abillity3End();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Support1();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Support2();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Hit();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void ChangeMode();


	/** Called for movement input */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Look(const FInputActionValue& Value);

	void LookNotMove();
public:
	UPROPERTY(EditAnyWhere, BlueprintReadwrite)
		bool bIsAttacking;
	UPROPERTY(EditAnyWhere, BlueprintReadwrite)
		bool bIsEvade;
	UPROPERTY(EditAnyWhere, BlueprintReadwrite)
		bool bIsArmored;

	UPROPERTY(EditAnyWhere, BlueprintReadwrite)
		bool bIsMobileMode;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* AttackComboMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* Abillity1Montage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* Abillity2Montage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* Abillity3Montage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* Skill1Montage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* Skill2Montage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EvadeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* HitReactionMontage;

	UPROPERTY(ReplicatedUsing = OnRep_SetSectionNameNone)
		FName AnimNextSectionName;

	UPROPERTY(BlueprintReadOnly)
		float Skill1CoolTime;

	UPROPERTY(EditAnyWhere,BlueprintReadwrite)
		float Skill1MaxCoolTime;

	UPROPERTY(BlueprintReadOnly)
		float Skill2CoolTime;

	UPROPERTY(EditAnyWhere,BlueprintReadwrite)
		float Skill2MaxCoolTime;

	UPROPERTY(BlueprintReadOnly)
		float Support1CoolTime;

	UPROPERTY(EditAnyWhere,BlueprintReadwrite)
		float Support1MaxCoolTime;

	UPROPERTY(BlueprintReadOnly)
		float Support2CoolTime;

	UPROPERTY(EditAnyWhere,BlueprintReadwrite)
		float Support2MaxCoolTime;

	UPROPERTY(BlueprintReadOnly)
		float EvadeCoolTime;

	UPROPERTY(EditAnyWhere,BlueprintReadwrite)
		float EvadeMaxCoolTime;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		UParticleSystem* hitParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		USoundBase* HitSound;
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadwrite)
		FDamageInfo CurrentDamageInfo;
public:
	UFUNCTION(Server, Reliable,BlueprintCallable, Category = "Weapon")
		virtual void CollisionTakeDamage(AActor* otherActor ,FDamageInfo DamageInfo) override;
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapon")
		virtual void SetCurrentDamageInfo(FDamageInfo DamageInfo) override;
	UFUNCTION(Client, Reliable, Category = "Damage")
		void OnWeaponOverlapClient(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Server, Reliable, Category = "Damage")
		void OnWeaponOverlapServer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Client, Reliable,BlueprintCallable, Category = "Weapon")
		virtual void SetWeaponCollision(ECollisionChannel Channel, ECollisionResponse NewResponse) override;
	UFUNCTION(Client, Reliable,BlueprintCallable, Category = "Weapon")
		virtual void SetSpAttackCollision(ECollisionChannel Channel, ECollisionResponse NewResponse) override;
	UFUNCTION(Server, Reliable,BlueprintCallable, Category = "Weapon")
		void SetEvadeCollision(ECollisionResponse NewResponse);
protected:
	UFUNCTION(BlueprintCallable)
		void SpawnSideKick();
	UFUNCTION(Server, Reliable, Category = "SideKick")
		void SidekickTakeDamage(AActor* otherActor, FDamageInfo DamageInfo);
	UFUNCTION(Server, Reliable, Category = "SideKick")
		void SidekickApplyDamage(AActor* otherActor);
	UFUNCTION(Category = "SideKick")
		void SidekickTakeDamage_Client(AActor* otherActor, FDamageInfo DamageInfo);
	UFUNCTION(Category = "SideKick")
		void SidekickApplyDamage_Client(AActor* otherActor);
	UFUNCTION(BlueprintCallable)
		void BindSidekickfunction();
	UFUNCTION(Server, Reliable, Category = "SideKick")
		void Sidekick1ActivateSupport();
	UFUNCTION(Server, Reliable, Category = "SideKick")
		void Sidekick1ActivateSupportfalse();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
	class AMapDestinationCheckActor* MapDestinationCheckActor;
	FVector Destinationloaction;
	int NumofDestination;
	void CheckDestination();
	void DestinationDeActive();
	void DestinationActive();
};
