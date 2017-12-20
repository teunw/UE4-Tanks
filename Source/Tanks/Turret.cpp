// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Turret.h"
#include "Tanks.h"
#include "PaperSpriteComponent.h"
#include "Tank.h"

const FName ATurret::MuzzleSocketName(TEXT("Muzzle"));

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = TurretDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TurretDirection"));

	TurretSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TurretSprite"));
	TurretSprite->SetupAttachment(TurretDirection);

	YawSpeed = 180.0f;
	Fire1Cooldown = 1.0f;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	check(GetParentComponent());
	Tank = Cast<ATank>(GetParentComponent()->GetOwner());
	check(Tank);
}

// Called every frame
void ATurret::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	// Aim!
	check(TurretDirection);
	if (Tank)
	{
		if (APlayerController* PC = Cast<APlayerController>(Tank->GetController()))
		{
			FVector2D AimLocation;
			if (PC->GetMousePosition(AimLocation.X, AimLocation.Y))
			{
				FVector2D TurretLocation = FVector2D::ZeroVector;
				UGameplayStatics::ProjectWorldToScreen(PC, TurretDirection->GetComponentLocation(), TurretLocation);
				float DesiredYaw;
				if (UTankStatics::FindLookAtAngle2D(TurretLocation, AimLocation, DesiredYaw))
				{
					FRotator CurrentRotation = TurretDirection->GetComponentRotation();
					float DeltaYaw = UTankStatics::FindDeltaAngleDegrees(CurrentRotation.Yaw, DesiredYaw);
					float MaxDeltaYawThisFrame = YawSpeed * DeltaSeconds;

					// Perform the current frame's rotation.
					if (MaxDeltaYawThisFrame > FMath::Abs(DeltaYaw))
					{
						// We can get all the way to the desired angle, so finish the rotation.
						CurrentRotation.Yaw = DesiredYaw;
					}
					else
					{
						// We can't turn as far as we want, so turn as far as we can in that direction.
						CurrentRotation.Yaw += (MaxDeltaYawThisFrame * FMath::Sign(DeltaYaw));
					}
					TurretDirection->SetWorldRotation(CurrentRotation);
				}
			}
		}

		// Handle input.
		const FTankInput& CurrentInput = Tank->GetCurrentInput();
		if (CurrentInput.bFire1 && Projectile)
		{
			if (UWorld* World = GetWorld())
			{
				float CurrentTime = World->GetTimeSeconds();
				if (Fire1ReadyTime <= CurrentTime)
				{
					FVector Loc = TurretSprite->GetSocketLocation(MuzzleSocketName);
					FRotator Rot = TurretDirection->GetComponentRotation();

					if (AActor* NewProjectile = World->SpawnActor(Projectile))
					{
						NewProjectile->SetActorLocation(Loc);
						NewProjectile->SetActorRotation(Rot);
					}

					// Set the cooldown timer.
					Fire1ReadyTime = CurrentTime + Fire1Cooldown;
				}
			}
		}
	}
}
