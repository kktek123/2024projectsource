// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "PlayerCharacter.h"
#include "DamageSystem.h"
#include "Kismet/GameplayStatics.h"
#include "../Action100ProjectGameMode.h"
#include "AICharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TestMpProjectile.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxStunValue = 100.0f;
	CurrentStunValue = MaxStunValue;
	AbillityTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("AbillityTrigger"));
	AbillityTrigger->SetupAttachment(RootComponent);

	DamageSystemComponent = CreateDefaultSubobject<UDamageSystem>(TEXT("DamageSystem"));
	DamageSystemComponent->SetIsReplicated(true);

	WeaponMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipWeapon_R"));
	WeaponMesh_R->SetupAttachment(GetMesh(), "hand_r");
	WeaponCollision_R = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision_R"));
	WeaponCollision_R->SetupAttachment(WeaponMesh_R);
	SpAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpAttackCollision"));
	SpAttackCollision->SetupAttachment(GetMesh());
	SpAttackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);

	SpParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpParticleSystemComponent"));
	SpParticleSystemComponent->SetupAttachment(GetMesh());
	//SpParticleSystemComponent->SetActive(false);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleSystemClass(TEXT("/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_ImpactSpark.P_ImpactSpark"));
	if (HitParticleSystemClass.Succeeded())
	{
		hitParticleSystem = HitParticleSystemClass.Object;
	}

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_SimulatedProxy) {
			WeaponCollision_R->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnWeaponOverlapClient);
			SpAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnWeaponOverlapClient);

	}

	AbillityTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnAbillityBeginOverlap);
	AbillityTrigger->OnComponentEndOverlap.AddDynamic(this, &AAICharacter::OnAbillityEndOverlap);
	DamageSystemComponent->OnStun.AddUObject(this, &AAICharacter::StunCoolTime);
}



// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::Attack()
{
	CheckAnimPlay(AttackMontage);
	HandleAnimtion(AttackMontage, "None");
}

void AAICharacter::Abillity1()
{
}

void AAICharacter::Reset()
{
}

float AAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//SetCurrentHealth(DamageAmount);
	FDamageInfo DamageInfo;
	DamageInfo.Amount = DamageAmount;
	DamageInfo.DamageType = E_DamageType::Melee;
	DamageInfo.DamageResponse = E_DamageResponse::HitReaction;
	DamageSystemComponent->TakeDamage(DamageInfo);
	CurrentStunValue -= 50;
	SetActorRotation(FRotator(0,UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation()).Yaw,0));
	if (DamageSystemComponent->bIsDead)
	{
		Death();
	}
	else
	{
		HandleAnimtion(HitReactionMontage, "None");

	}
	bIsDamaged = true;
	return DamageAmount;
}

void AAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//현재 체력 리플리케이트
	DOREPLIFETIME(AAICharacter, AbillityTrigger);
	DOREPLIFETIME(AAICharacter, bIsStun);
}
void AAICharacter::HandleAnimtion(UAnimMontage* Montage, FName SectionName)
{
	PlayAnimMontage(Montage, 1, SectionName);
	FOnMontageBlendingOutStarted CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AAICharacter::MontageComplete);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, Montage);

	HandleAnimtionServer(Montage, SectionName);
}

void AAICharacter::HandleAnimtionServer_Implementation(UAnimMontage* Montage, FName SectionName)
{
	PlayAnimMontage(Montage, 1, SectionName);
	HandleAnimtionMulticast(Montage, SectionName);
}

void AAICharacter::HandleAnimtionMulticast_Implementation(UAnimMontage* Montage, FName SectionName)
{
	if (!IsLocallyControlled())
	{
		PlayAnimMontage(Montage, 1, SectionName);
	}

}

void AAICharacter::CheckAnimPlay(UAnimMontage* CurrentAnimMontage)
{
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentAnimMontage))
	{
		AnimNextSectionName = "None";
	}
}

void AAICharacter::StunCoolTime()
{
	if (!bIsStun)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
			FString::Printf(TEXT("Stun")));
		Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			->CheckTargetAbillity(0);
		bIsStun = true;
		Cast<AAICharacterController>(GetController())->GetBlackboardComponent()->SetValueAsBool("Stun", true);
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AAICharacter::StunRecover
			, 3.0f, false);

	}
}

void AAICharacter::StunRecover()
{
	bIsStun = false;
	Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		->CheckTargetAbillity(0);
	DamageSystemComponent->Stun = 0;
	Cast<AAICharacterController>(GetController())->GetBlackboardComponent()->SetValueAsBool("Stun", false);
}

void AAICharacter::ShotFire_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		FVector spawnLocation = GetActorLocation() + (GetControlRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
		FRotator spawnRotation = GetControlRotation();
	
		FActorSpawnParameters spawnParameters;
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.Owner = this;
	
		Owner->GetWorld()->SpawnActor<ATestMpProjectile>(spawnedProjectile,spawnLocation, spawnRotation, spawnParameters);
	
	}

}

void AAICharacter::MontageComplete(class UAnimMontage* montage, bool bInterrupted)
{
	OnEndAnim.Broadcast();
}

void AAICharacter::CollisionOverlap()
{
	AbillityTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	ActivateAbillity();
}

void AAICharacter::OnAbillityTrigger(AActor* OtherActor, float Radius)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (IsValid(Player))
	{
		Player->CheckTargetAbillity(Radius);
	}
}

void AAICharacter::OnAbillityBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnAbillityTrigger(OtherActor, AbillityTrigger->GetScaledSphereRadius() + 50);
}

void AAICharacter::OnAbillityEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnAbillityTrigger(OtherActor,0);
}

void AAICharacter::ActivateAbillity_Implementation()
{
	AbillityTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	bIsStun = true;
	ActivateAbillityMulticast();
}

void AAICharacter::ActivateAbillityMulticast_Implementation()
{
	AbillityTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	bIsStun = true;

}

void AAICharacter::DeactivateAbillity_Implementation()
{
	AbillityTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

}

void AAICharacter::Death_Implementation()
{
	if (IsValid(DeathMontage))
	{
		HandleAnimtion(DeathMontage, "None");
		SetLifeSpan(DeathMontage->GetPlayLength() * 0.8f);

	}
}

void AAICharacter::SetWeaponCollision_Implementation(ECollisionChannel Channel, ECollisionResponse NewResponse)
{
	WeaponCollision_R->SetCollisionResponseToChannel(Channel, NewResponse);
}

void AAICharacter::SetSpAttackCollision_Implementation(ECollisionChannel Channel, ECollisionResponse NewResponse)
{
	SpAttackCollision->SetCollisionResponseToChannel(Channel, NewResponse);
}

void AAICharacter::CollisionTakeDamage_Implementation(AActor* otherActor, FDamageInfo DamageInfo)
{
	AAICharacterController* AIController = Cast<AAICharacterController>(GetController());
	OnTakeDamage(otherActor, DamageInfo);
	if (DamageSystemComponent->bIsDead)
	{
		if (!bIsDead)
		{
			bIsDead = true;
			SetActorRotation(FRotator(0, DamageInfo.PointDegree, 0));
			if (IsValid(AIController))
				AIController->GetBlackboardComponent()->SetValueAsBool("Death", true);
			//Death();
		}
	}
	else
	{
		SetActorRotation(FRotator(0, DamageInfo.PointDegree, 0));
		if (IsValid(AIController))
			AIController->GetBlackboardComponent()->SetValueAsBool("Hit", true);
		//HandleAnimtion(HitReactionMontage, "None");
	}

	if (IsValid(AIController)) {
		AIController->GetBlackboardComponent()->SetValueAsObject("EnemyActor", otherActor);
	}

}

void AAICharacter::OnWeaponOverlapClient_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CurrentDamageInfo.PointDegree = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), GetActorLocation()).Yaw;
	OnApplyDamage(OtherActor, CurrentDamageInfo);
	UGameplayStatics::SpawnEmitterAtLocation(this, hitParticleSystem, OverlappedComponent->GetComponentLocation(), FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, OverlappedComponent->GetComponentLocation(), FRotator::ZeroRotator);
}


void AAICharacter::SetCurrentDamageInfo(FDamageInfo DamageInfo)
{
	CurrentDamageInfo = DamageInfo;
	OnRep_CurrentDamageInfo();
}

void AAICharacter::OnRep_CurrentDamageInfo()
{
}

void AAICharacter::OnTakeDamage(AActor* OtherActor, FDamageInfo DamageInfo)
{
}

void AAICharacter::OnApplyDamage(AActor* OtherActor, FDamageInfo DamageInfo)
{
}
