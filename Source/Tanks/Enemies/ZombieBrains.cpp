// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ZombieBrains.h"
#include "Tanks.h"
#include "Zombie.h"


void AZombieBrains::Tick(float DeltaTime)
{
	if (PawnAsZombie)
	{
		if (AActor* Target = PawnAsZombie->GetTarget())
		{
			// We do have a target. Shamble toward it and attempt violence!
			FVector DirectionToTarget = (Target->GetActorLocation() - PawnAsZombie->GetActorLocation()).GetSafeNormal2D();
			float DotToTarget = FVector::DotProduct(DirectionToTarget, PawnAsZombie->GetActorForwardVector());
			float SidewaysDotToTarget = FVector::DotProduct(DirectionToTarget, PawnAsZombie->GetActorRightVector());
			float DeltaYawDesired = FMath::Atan2(SidewaysDotToTarget, DotToTarget);

			if (PawnAsZombie->ZombieAIShouldAttack())
			{
				PawnAsZombie->AddAttackInput();
			}
			else
			{
				// Move faster when facing toward the target so that we turn more accurately/don't orbit.
				PawnAsZombie->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), FMath::GetMappedRangeValueClamped(FVector2D(-0.707f, 0.707f), FVector2D(0.0f, 1.0f), DotToTarget));
				// Attempt the entire turn in one frame. The Zombie itself will cap this, we're only expressing our desired turn amount here.
				PawnAsZombie->AddRotationInput(DeltaYawDesired);
			}
		}
	}
}

void AZombieBrains::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	PawnAsZombie = Cast<AZombie>(GetPawn());
}

void AZombieBrains::UnPossess()
{
	Super::UnPossess();
	PawnAsZombie = nullptr;
}