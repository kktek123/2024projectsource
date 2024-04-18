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

	// �ݸ��� �׽�Ʈ�� ���Ǵ� ���Ǿ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* SphereComponent;

	// ������Ʈ�� ���־� ǥ���� �����ϴ� ����ƽ �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* StaticMesh;

	// �߻�ü �������� ó���ϴ� �����Ʈ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UProjectileMovementComponent* ProjectileMovementComponent;

	// �߻�ü�� �ٸ� ������Ʈ�� ������ ��ġ�� ������ �� ���Ǵ� ��ƼŬ
	UPROPERTY(EditAnywhere, Category = "Effects")
		class UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		FDamageInfo HitDamageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		USoundBase* ExplosionSound;

	void CollisionOverlap();
};
