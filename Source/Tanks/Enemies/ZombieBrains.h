// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AIController.h"
#include "ZombieBrains.generated.h"

class AZombie;

/**
 * 
 */
UCLASS()
class TANKS_API AZombieBrains : public AAIController
{
	GENERATED_BODY()
public:

	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;

protected:
	// Our pawn, pre-cast to a Zombie. Will be NULL if the pawn is NULL or is not a Zombie.
	UPROPERTY(BlueprintReadOnly, Category = "Zombie")
	AZombie* PawnAsZombie;
};
