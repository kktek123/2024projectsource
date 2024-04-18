// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_RunEQS.h"
#include "BTService_RunEQSStrafeAround.generated.h"

/**
 * 
 */
UCLASS()
class ACTION100PROJECT_API UBTService_RunEQSStrafeAround : public UBTService_RunEQS
{
	GENERATED_BODY()
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	void Strafe();

public:
	UFUNCTION(BlueprintPure)
		class UEnvQuery* GetStrafeQuery();
public:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere)
		bool UseRandomDirection;

	UBlackboardComponent* MyBlackboardComponent;
private:

	class UEnvQuery* QueryTemplate;
	APawn* ControlledPawn;
	class UEnvQuery* RightQuery;
	class UEnvQuery* LeftQuery;


};
