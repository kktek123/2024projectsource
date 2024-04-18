// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMpProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "EpuipFunctionInterface.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATestMpProjectile::ATestMpProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//발사체와 콜리전의 루트 컴포넌트 역할을 할 SphereComponent 정의
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(37.5f);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = SphereComponent;

	//비주얼 표현을 담당할 메시 정의
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	//사용할 메시 에셋이 발견되면 스태틱 메시와 위치/스케일 설정
	if (DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -37.5f));
		StaticMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect.Object;
	}

	//발사체 무브먼트 컴포넌트 정의
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	//DamageType = UDamageType::StaticClass();
	//Damage = 10.0f;

	//발사체 충돌 함수를 히트 이벤트에 등록
	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATestMpProjectile::OnProjectileImpact);
	}
}

// Called when the game starts or when spawned
void ATestMpProjectile::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATestMpProjectile::CollisionOverlap
		, 0.01f, false);

}

void ATestMpProjectile::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, spawnLocation, FRotator::ZeroRotator);

}

void ATestMpProjectile::OnProjectileImpact_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (IsValid(OtherActor))
	//{
	//	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
	//}
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		IEpuipFunctionInterface* Interface = Cast<IEpuipFunctionInterface>(OtherActor);
		if (Interface != nullptr) {
			HitDamageInfo.PointDegree = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), GetActorLocation()).Yaw;
			Interface->CollisionTakeDamage(this, HitDamageInfo);
		}
	}
	Destroy();
}

// Called every frame
void ATestMpProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestMpProjectile::CollisionOverlap()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

