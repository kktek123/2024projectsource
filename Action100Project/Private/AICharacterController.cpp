// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterController.h"
#include "Kismet/KismetMathLibrary.h"

void AAICharacterController::AddCheckedEnemyActors(AActor* addactor)
{
	bool isInArray = false;
	for (AActor* checkActor : CheckedEnemyActors)
	{
		if (addactor == checkActor)
			isInArray = true;
	}
	if (!isInArray)
		CheckedEnemyActors.Add(addactor);
}

AActor* AAICharacterController::SetTargetActor(float dist)
{
	float distance = dist;
	AActor* TargetActor = nullptr;
	for (AActor* checkActor : CheckedEnemyActors)
	{
		if (IsValid(checkActor))
		{
			if (distance > UKismetMathLibrary::Vector_Distance(GetOwner()->GetActorLocation(), checkActor->GetActorLocation()))
			{
				distance = UKismetMathLibrary::Vector_Distance(GetOwner()->GetActorLocation(), checkActor->GetActorLocation());
				TargetActor = checkActor;
			}

		}
	}

	return TargetActor;
}
