// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "PlayerAnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Action100ProjectGameMode.h"
#include "EnemyCharacter.h"
#include "SidekickCharacter.h"
#include "DamageSystem.h"
#include "SidkickCharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"
#include "LevelSpawnActor.h"
#include "MapDestinationCheckActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WeaponMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipWeapon_R"));
	WeaponMesh_R->SetupAttachment(GetMesh(), "hand_r");
	WeaponCollision_R = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision_R"));
	WeaponCollision_R->SetupAttachment(WeaponMesh_R);
	WeaponCollision_R->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlapClient);


	WeaponMesh_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipWeapon_L"));
	WeaponMesh_L->SetupAttachment(GetMesh(), "hand_l");
	WeaponCollision_L = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision_L"));
	WeaponCollision_L->SetupAttachment(WeaponMesh_L);
	WeaponCollision_L->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlapClient);

	SpAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpAttackCollision"));
	SpAttackCollision->SetupAttachment(GetMesh());

	DestinationArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("DestinationArm"));
	DestinationArm->SetupAttachment(RootComponent);


	DamageSystemComponent = CreateDefaultSubobject<UDamageSystem>(TEXT("DamageSystem"));
	DamageSystemComponent->SetIsReplicated(true);
	bIsAttacking = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleSystemClass(TEXT("/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_ImpactSpark.P_ImpactSpark"));
	if (HitParticleSystemClass.Succeeded())
	{
		hitParticleSystem = HitParticleSystemClass.Object;
	}
	Skill1MaxCoolTime = 10.f;
	Skill2MaxCoolTime = 10.f;
	Support1MaxCoolTime = 10.f;
	Support2MaxCoolTime = 10.f;
	EvadeMaxCoolTime = 3.f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	SpAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlapClient);
	Skill1CoolTime = Skill1MaxCoolTime;
	Skill2CoolTime = Skill2MaxCoolTime;
	Support1CoolTime = Support1MaxCoolTime;
	Support2CoolTime = Support2MaxCoolTime;
	EvadeCoolTime = EvadeMaxCoolTime;
	if (GetLocalRole() == ROLE_Authority) {
		SpawnSideKick();
	}
	//FTimerHandle timerHandle;
	//GetWorldTimerManager().SetTimer(timerHandle, this, &APlayerCharacter::BindSidekickfunction, 2.0f, false);
	CheckDestination();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(Cast<AEnemyCharacter>(TargetActor)))
	{
		if (Cast<AEnemyCharacter>(TargetActor)->DamageSystemComponent->bIsDead == false)
		{
			if (bIsDashMove)
			{
				TargetDashMove(TargetActor->GetActorLocation(), DeltaTime, 1500);
			}
			if(bIsAttacking)
			LockRotateTarget(TargetActor->GetActorLocation());
		}
	}
	else if(!IsValid(TargetActor))
	{
		CheckTarget();
	}
	if (Skill1CoolTime < Skill1MaxCoolTime)
	{
		Skill1CoolTime += DeltaTime;
	}
	if (Skill2CoolTime < Skill2MaxCoolTime)
	{
		Skill2CoolTime += DeltaTime;
	}
	if (Support1CoolTime < Support1MaxCoolTime)
	{
		Support1CoolTime += DeltaTime;
	}
	if (Support2CoolTime < Support2MaxCoolTime)
	{
		Support2CoolTime += DeltaTime;
	}
	if (EvadeCoolTime < EvadeMaxCoolTime)
	{
		EvadeCoolTime += DeltaTime;
	}
	
	DestinationArm->SetWorldRotation(FRotator(0, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Destinationloaction).Yaw, 0));
}


void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//현재 체력 리플리케이트
	DOREPLIFETIME(APlayerCharacter, AnimNextSectionName);
	DOREPLIFETIME(APlayerCharacter, TargetActor);
	DOREPLIFETIME(APlayerCharacter, bIsCanAbillity);
	DOREPLIFETIME(APlayerCharacter, Sidekick1);
	DOREPLIFETIME(APlayerCharacter, CurrentDamageInfo);
}


float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FDamageInfo DamageInfo;
	DamageInfo.Amount = DamageAmount;
	DamageInfo.DamageType = E_DamageType::Melee;
	DamageInfo.DamageResponse = E_DamageResponse::HitReaction;
	DamageSystemComponent->TakeDamage(DamageInfo);
	SetActorRotation(FRotator(0, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation()).Yaw, 0));
	return DamageAmount;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::None, this, &APlayerCharacter::LookNotMove);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
		EnhancedInputComponent->BindAction(EvadeAction, ETriggerEvent::Started, this, &APlayerCharacter::Evade);
		EnhancedInputComponent->BindAction(Abillity1Action, ETriggerEvent::Started, this, &APlayerCharacter::Abillity1);
		EnhancedInputComponent->BindAction(Abillity2Action, ETriggerEvent::Started, this, &APlayerCharacter::Abillity2);
		EnhancedInputComponent->BindAction(Abillity3Action, ETriggerEvent::Started, this, &APlayerCharacter::Abillity3);
		EnhancedInputComponent->BindAction(Skill1Action, ETriggerEvent::Started, this, &APlayerCharacter::Skill1);
		EnhancedInputComponent->BindAction(Skill2Action, ETriggerEvent::Started, this, &APlayerCharacter::Skill2);
		EnhancedInputComponent->BindAction(Support1Action, ETriggerEvent::Started, this, &APlayerCharacter::Support1);
		EnhancedInputComponent->BindAction(Support2Action, ETriggerEvent::Started, this, &APlayerCharacter::Support2);
		EnhancedInputComponent->BindAction(ChangeModeAction, ETriggerEvent::Started, this, &APlayerCharacter::ChangeMode);
	}
}

void APlayerCharacter::MontageComplete(UAnimMontage* montage, bool bInterrupted)
{
	CheckTargetAbillity(0);
	bIsAttacking = false;
	bIsEvade = false;
	bIsArmored = false;
}

void APlayerCharacter::Attack()
{
	if (!bIsAttacking)
	{
		CheckTarget();
		CheckAnimPlay(AttackComboMontage);
		FRotator rot = FRotator(0, GetControlRotation().Yaw, 0);
		if (IsValid(TargetActor))
		{
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()));
		}
		//PlayAnimMontage(AttackComboMontage, 1, AnimNextSectionName);
		HandleAnimtion(AttackComboMontage, AnimNextSectionName);
		bIsAttacking = true;
	}
}



void APlayerCharacter::AttackEnd()
{
}

void APlayerCharacter::Evade()
{
	if (!bIsEvade && EvadeCoolTime >= EvadeMaxCoolTime)
	{
		EvadeCoolTime = 0;
		CheckAnimPlay(EvadeMontage);
		//PlayAnimMontage(EvadeMontage, 1, AnimNextSectionName);
		HandleAnimtion(EvadeMontage, AnimNextSectionName);
		bIsEvade = true;
	}
}

void APlayerCharacter::EvadeEnd()
{
}

void APlayerCharacter::Skill1()
{
	if (!bIsAttacking && Skill1CoolTime >= Skill1MaxCoolTime)
	{
		Skill1CoolTime = 0;
		CheckTarget();
		CheckAnimPlay(Skill1Montage);
		if (IsValid(TargetActor))
		{
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()));
			bLockTarget = true;
		}
		//PlayAnimMontage(Skill1Montage, 1, AnimNextSectionName);
		HandleAnimtion(Skill1Montage, AnimNextSectionName);
		bIsAttacking = true;
		bIsArmored = true;
	}
}

void APlayerCharacter::Skill1End()
{
}

void APlayerCharacter::Skill2()
{
	if (!bIsAttacking && Skill2CoolTime >= Skill2MaxCoolTime)
	{
		Skill2CoolTime = 0;
		CheckTarget();
		CheckAnimPlay(Skill2Montage);
		if (IsValid(TargetActor))
		{
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()));
			bLockTarget = true;
		}
		//PlayAnimMontage(Skill2Montage, 1, AnimNextSectionName);
		HandleAnimtion(Skill2Montage, AnimNextSectionName);
		bIsAttacking = true;
		bIsArmored = true;
	}
}

void APlayerCharacter::Skill2End()
{
}

void APlayerCharacter::Abillity1()
{
	if (bIsCanAbillity)
	{
		CheckTarget();
		CheckAnimPlay(Abillity1Montage);
		CheckTargetAbillity(0);
		if (IsValid(TargetActor))
		{
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()));
			bLockTarget = true;
		}
		//PlayAnimMontage(Abillity1Montage, 1, AnimNextSectionName);
		HandleAnimtion(Abillity1Montage, AnimNextSectionName);
		bIsArmored = true;
		OnRep_SetIsCanAbillity(false);
	}
}

void APlayerCharacter::Abillity1End()
{
}

void APlayerCharacter::Abillity2()
{
	if (bIsCanAbillity)
	{
		CheckTarget();
		CheckAnimPlay(Abillity2Montage);
		CheckTargetAbillity(0);
		if (IsValid(TargetActor))
		{
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()));
			bLockTarget = true;
		}
		bIsArmored = true;
		//PlayAnimMontage(Abillity2Montage, 1, AnimNextSectionName);
		HandleAnimtion(Abillity2Montage, AnimNextSectionName);

		OnRep_SetIsCanAbillity(false);
	}
}

void APlayerCharacter::Abillity2End()
{
}

void APlayerCharacter::Abillity3()
{
	if (bIsCanAbillity)
	{
		CheckTarget();
		CheckAnimPlay(Abillity3Montage);
		CheckTargetAbillity(0);
		if (IsValid(TargetActor))
		{
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()));
			bLockTarget = true;
		}
		bIsArmored = true;
		//PlayAnimMontage(Abillity3Montage, 1, AnimNextSectionName);
		HandleAnimtion(Abillity3Montage, AnimNextSectionName);

		OnRep_SetIsCanAbillity(false);
	}
}

void APlayerCharacter::Abillity3End()
{
}

void APlayerCharacter::Support1()
{
	if (IsValid(Sidekick1) && Support1CoolTime >= Support1MaxCoolTime)
	{
		Support1CoolTime = 0;
		Sidekick1ActivateSupport();
	}
}

void APlayerCharacter::Support2()
{
}

void APlayerCharacter::Hit()
{
	if (IsValid(TargetActor))
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()));
		bLockTarget = true;
	}
	if(!bIsArmored)
	HandleAnimtion(HitReactionMontage, AnimNextSectionName);
}

void APlayerCharacter::ChangeMode()
{
	bIsMobileMode = !bIsMobileMode;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bIsAttacking)
		return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	bIsLookValueZero = false;
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void APlayerCharacter::LookNotMove()
{
	bIsLookValueZero = true;
}


void APlayerCharacter::OnRep_SetSectionNameNone()
{
	if (IsValid(TargetActor))
	{
		if (UKismetMathLibrary::Vector_Distance(GetActorLocation(), TargetActor->GetActorLocation()) > 100.0f)
		{
			AnimNextSectionName = "Move";
		}
		else
		{
			AnimNextSectionName = "None";
		}
	}
	else
	{
		AnimNextSectionName = "None";
	}
}

void APlayerCharacter::OnRep_SetIsCanAbillity_Implementation(bool b)
{
	bIsCanAbillity = b;
}

void APlayerCharacter::CheckTarget_Implementation()
{
	AAction100ProjectGameMode* GameMode = Cast<AAction100ProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	float distance = 1000.0f;
	if (IsValid(GameMode))
	{
		for (AEnemyCharacter* Enemy : GameMode->Enemys)
		{
			if (IsValid(Enemy))
			{
				if (!Enemy->DamageSystemComponent->bIsDead)
				{
					if (distance > UKismetMathLibrary::Vector_Distance(GetActorLocation(), Enemy->GetActorLocation()))
					{
						distance = UKismetMathLibrary::Vector_Distance(GetActorLocation(), Enemy->GetActorLocation());
						TargetActor = Enemy;
					}

				}

			}
		}

	}
}

void APlayerCharacter::CheckTargetAbillity_Implementation(float TriggerRadius)
{
	AAction100ProjectGameMode* GameMode = Cast<AAction100ProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	float distance = ActiveTriggerdistance;
	if (TriggerRadius) {
		distance = TriggerRadius;
	}
	if (IsValid(GameMode))
	{
		bool bTargetInRange = false;
		for (AEnemyCharacter* Enemy : GameMode->Enemys)
		{
			if (IsValid(Enemy))
			{
				if (!Enemy->DamageSystemComponent->bIsDead)
				{
					if (Enemy->bIsStun) {
						if (distance > UKismetMathLibrary::Vector_Distance(GetActorLocation(), Enemy->GetActorLocation()))
						{
							distance = UKismetMathLibrary::Vector_Distance(GetActorLocation(), Enemy->GetActorLocation());
							TargetActor = Enemy;
							bTargetInRange = true;
						}
					}
				}

			}
		}
		if (bTargetInRange)
		{
			bIsCanAbillity = true;
		}
		else {
			bIsCanAbillity = false;
		}
	}
}

void APlayerCharacter::HandleAnimtion_Implementation(UAnimMontage* Montage, FName SectionName)
{
	CheckAnimPlay(Montage);
	PlayAnimMontage(Montage, 1, SectionName);
	HandleAnimtionMulticast(Montage, SectionName);
	FOnMontageBlendingOutStarted CompleteDelegate;
	CompleteDelegate.BindUObject(this, &APlayerCharacter::MontageComplete);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, Montage);

}

void APlayerCharacter::HandleAnimtionMulticast_Implementation(UAnimMontage* Montage, FName SectionName)
{
	PlayAnimMontage(Montage, 1, SectionName);
}


void APlayerCharacter::CheckAnimPlay(UAnimMontage* CurrentAnimMontage)
{
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentAnimMontage))
	{
		OnRep_SetSectionNameNone();
	}

}


void APlayerCharacter::LockRotateTarget_Implementation(FVector TargetLocation)
{
	if (!bIsEvade)
	SetActorRotation(FRotator(0, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation).Yaw, 0));
}


void APlayerCharacter::TargetDashMove_Implementation(FVector TargetLocation, float Delta, float speed)
{
	FVector MyLocation = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), TargetLocation, Delta, speed);
	SetActorLocation(MyLocation);
}

void APlayerCharacter::SetWeaponCollision_Implementation(ECollisionChannel Channel, ECollisionResponse NewResponse)
{
	if (GetLocalRole() == ROLE_AutonomousProxy) {
		WeaponCollision_R->SetCollisionResponseToChannel(Channel, NewResponse);
		WeaponCollision_L->SetCollisionResponseToChannel(Channel, NewResponse);
	}
}

void APlayerCharacter::SetEvadeCollision_Implementation(ECollisionResponse NewResponse)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel2, NewResponse);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, NewResponse);

}

void APlayerCharacter::SetSpAttackCollision_Implementation(ECollisionChannel Channel, ECollisionResponse NewResponse)
{
	SpAttackCollision->SetCollisionResponseToChannel(Channel, NewResponse);
}

void APlayerCharacter::CollisionTakeDamage_Implementation(AActor* otherActor,FDamageInfo DamageInfo)
{
	if (IsValid(Cast<AAICharacter>(TargetActor)))
	{
		TargetActor = otherActor;
	}
	DamageSystemComponent->TakeDamage(DamageInfo);
	Hit();
}

void APlayerCharacter::OnWeaponOverlapClient_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnWeaponOverlapServer(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UGameplayStatics::SpawnEmitterAtLocation(this, hitParticleSystem, OverlappedComponent->GetComponentLocation(), FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, OverlappedComponent->GetComponentLocation(), FRotator::ZeroRotator);
}

void APlayerCharacter::OnWeaponOverlapServer_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IEpuipFunctionInterface* Interface = Cast<IEpuipFunctionInterface>(OtherActor);
	if (Interface != nullptr) {
		CurrentDamageInfo.PointDegree = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), GetActorLocation()).Yaw;
		Interface->CollisionTakeDamage(this, CurrentDamageInfo);
	}

}

void APlayerCharacter::SidekickTakeDamage_Implementation(AActor* otherActor, FDamageInfo DamageInfo)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "SidekickTakeDamage_Server");
	Sidekick1->DamageSystemComponent->TakeDamage(DamageInfo);
	AAICharacterController* AIController = Cast<AAICharacterController>(Sidekick1->GetController());
	if (Sidekick1->DamageSystemComponent->bIsDead)
	{
		if (!Sidekick1->bIsDead)
		{
			Sidekick1->bIsDead = true;
			Sidekick1->SetActorRotation(FRotator(0, DamageInfo.PointDegree, 0));
			if (IsValid(AIController))
				AIController->GetBlackboardComponent()->SetValueAsBool("Death", true);
		}
	}
	else
	{
		Sidekick1->SetActorRotation(FRotator(0, DamageInfo.PointDegree, 0));
		if (IsValid(AIController))
			AIController->GetBlackboardComponent()->SetValueAsBool("Hit", true);
	}
	if (IsValid(AIController)) {
		AIController->GetBlackboardComponent()->SetValueAsObject("EnemyActor", otherActor);
	}
}

void APlayerCharacter::SidekickApplyDamage_Implementation(AActor* otherActor)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "SidekickApplyDamage_Server");
	if (GetLocalRole() == ROLE_Authority) {
		IEpuipFunctionInterface* Interface = Cast<IEpuipFunctionInterface>(otherActor);
		if (Interface != nullptr) {
			Interface->CollisionTakeDamage(Sidekick1, Sidekick1->CurrentDamageInfo);
		}
	}
}

void APlayerCharacter::SidekickTakeDamage_Client(AActor* otherActor, FDamageInfo DamageInfo)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "SidekickTakeDamage_Client");
	SidekickTakeDamage(otherActor, DamageInfo);
}

void APlayerCharacter::SidekickApplyDamage_Client(AActor* otherActor)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "SidekickApplyDamage_Client");
	SidekickApplyDamage(otherActor);
}

void APlayerCharacter::SpawnSideKick()
{
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.X -= 200;
	SpawnLocation.Y -= 200;
	Sidekick1 = GetWorld()->SpawnActor<ASidekickCharacter>(spawnChar, SpawnLocation, FRotator(0, 0, 0), FActorSpawnParameters());
	Sidekick1->SelfPlayer = this;
	AAICharacterController* AIController = Cast<AAICharacterController>(Sidekick1->GetController());
	if (IsValid(AIController)) {
		AIController->GetBlackboardComponent()->SetValueAsObject("SelfActor", this);
	}
}

void APlayerCharacter::BindSidekickfunction()
{
	if (IsValid(Sidekick1))
	{
		Sidekick1->OnTakeDamageDelegate.AddUObject(this, &APlayerCharacter::SidekickTakeDamage_Client);
		Sidekick1->OnApplyDamageDelegate.AddUObject(this, &APlayerCharacter::SidekickApplyDamage_Client);
	}
}

void APlayerCharacter::CheckDestination()
{
	if (IsValid(MapDestinationCheckActor))
	{
		if (NumofDestination < MapDestinationCheckActor->Destinationloaction.Num())
		{
			ALevelSpawnActor* Destination = MapDestinationCheckActor->Destinationloaction[NumofDestination];
		
			if (IsValid(Destination))
			{
				Destinationloaction = Destination->GetActorLocation();
				Destination->OnLevelStart.AddUObject(this, &APlayerCharacter::DestinationDeActive);
				Destination->OnLevelEnd.AddUObject(this, &APlayerCharacter::DestinationActive);
				DestinationArm->GetChildComponent(0)->SetVisibility(true);
			}
		}
	}
}

void APlayerCharacter::DestinationDeActive()
{
	DestinationArm->GetChildComponent(0)->SetVisibility(false);
}

void APlayerCharacter::DestinationActive()
{
	NumofDestination++;
	CheckDestination();
}


void APlayerCharacter::Sidekick1ActivateSupport_Implementation()
{
	ASidkickCharacterController* SidkickController = Cast<ASidkickCharacterController>(Sidekick1->GetController());
	if (IsValid(SidkickController))
	{
		SidkickController->ActivateSupport();
		SidkickController->GetBlackboardComponent()->SetValueAsBool("ActivateSupport", true);
		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, this, &APlayerCharacter::Sidekick1ActivateSupportfalse, 2.0f, false);
	}
}

void APlayerCharacter::Sidekick1ActivateSupportfalse_Implementation()
{
	ASidkickCharacterController* SidkickController = Cast<ASidkickCharacterController>(Sidekick1->GetController());
	if (IsValid(SidkickController)) {
		SidkickController->GetBlackboardComponent()->SetValueAsBool("ActivateSupport", false);
	}
}

void APlayerCharacter::SetCurrentDamageInfo_Implementation(FDamageInfo DamageInfo)
{
	CurrentDamageInfo = DamageInfo;
}
