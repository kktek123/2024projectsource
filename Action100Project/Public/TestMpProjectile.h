// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageSystem.h"
#include "TestMpProjectile.generated.h"

UCLASS()
class ACTION100PROJECT_API ATestMpProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestMpProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION(Client, Reliable, Category = "Projectile")
		void OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 콜리전 테스트에 사용되는 스피어 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* SphereComponent;

	// 오브젝트의 비주얼 표현을 제공하는 스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* StaticMesh;

	// 발사체 움직임을 처리하는 무브먼트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UProjectileMovementComponent* ProjectileMovementComponent;

	// 발사체가 다른 오브젝트에 영향을 미치고 폭발할 때 사용되는 파티클
	UPROPERTY(EditAnywhere, Category = "Effects")
		class UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		FDamageInfo HitDamageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		USoundBase* ExplosionSound;

	void CollisionOverlap();
};
