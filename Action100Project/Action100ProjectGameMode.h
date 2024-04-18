// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Action100ProjectGameMode.generated.h"


UCLASS(minimalapi)
class AAction100ProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAction100ProjectGameMode();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere,Blueprintreadwrite)
		TArray<class AEnemyCharacter*> Enemys;

};



