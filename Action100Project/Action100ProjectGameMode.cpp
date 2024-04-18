// Copyright Epic Games, Inc. All Rights Reserved.

#include "Action100ProjectGameMode.h"
#include "Action100ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Public/PlayerCharacter.h"
#include "Public/AICharacter.h"
#include "Public/EnemyCharacter.h"
#include "DamageSystem.h"

AAction100ProjectGameMode::AAction100ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAction100ProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
}
