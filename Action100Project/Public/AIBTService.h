// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AIBTService.generated.h"


UENUM(BlueprintType)
enum class EC_AIBehavior : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	RangeAttack UMETA(DisplayName = "RangeAttack"),
	MagicAttack UMETA(DisplayName = "MagicAttack"),
	Approach UMETA(DisplayName = "Approach"),
	Flee UMETA(DisplayName = "Flee"),
	StrafeAround UMETA(DisplayName = "StrafeAround"),
	Hit UMETA(DisplayName = "Hit"),
	Summon UMETA(DisplayName = "Summon"),
	Heal UMETA(DisplayName = "Heal"),
};
/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API UAIBTService : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void ReceiveTickAI(AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds);

	//UFUNCTION(BlueprintImplementableEvent, Category = AI)
	void ReceiveSearchStartAI(AAIController* OwnerController, APawn* ControlledPawn);
public:
	void OnStaminaValueChanged(float NewValue, float MaxValue);
	void Update(UBehaviorTreeComponent& OwnerComp);

	void UpdateBehavior(UBehaviorTreeComponent& OwnerComp);
	void UpdateActivities();
	FTimerHandle CountdownTimerHandle;

private:
	void SetBehavior(EC_AIBehavior Behavior);
	void Stamina();
public:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector BehaviorKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere)
		float MeleeAttackBehaviorRange;
	UPROPERTY(EditAnywhere)
		float RangedAttackBehaviorRange;
	UPROPERTY(EditAnywhere)
		float EnemyFollowRange;
	UPROPERTY(EditAnywhere)
		int32 CheckMaxCombo;
	float TicksNearTarget;
	bool IsOutOfStamina;
	//AActor* Target;
	UBlackboardComponent* MyBlackboardComponent;


};
