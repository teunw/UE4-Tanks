// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Tank.h"
#include "Tanks.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"


void FTankInput::Sanitize()
{
	MovementInput = RawMovementInput.ClampAxes(-1.0f, 1.0f);
	MovementInput.GetSafeNormal();
	RawMovementInput.Set(0.0f, 0.0f);
}

void FTankInput::MoveX(float AxisValue)
{
	RawMovementInput.X += AxisValue;
}

void FTankInput::MoveY(float AxisValue)
{
	RawMovementInput.Y += AxisValue;
}

void FTankInput::Fire1(bool bPressed)
{
	bFire1 = bPressed;
}

void FTankInput::Fire2(bool bPressed)
{
	bFire2 = bPressed;
}


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankBase"));
	}

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TankDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TankDirection"));
	TankDirection->SetupAttachment(RootComponent);

	TankSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankSprite"));
	TankSprite->SetupAttachment(TankDirection);

	ChildTurret = CreateDefaultSubobject<UChildActorComponent>(TEXT("Turret"));
	ChildTurret->SetupAttachment(TankDirection);

	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->CameraLagSpeed = 2.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	CameraComponent->OrthoWidth = 1024.0f;
	CameraComponent->AspectRatio = 4.0f / 3.0f;
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->SetWorldRotation(FRotator(-90.0f, -90.0f, 0.0f));

	MoveSpeed = 100.0f;
	MoveAccel = 200.0f;
	YawSpeed = 180.0f;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATank::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	TankInput.Sanitize();

	// Respond to controls.
	{
		FVector DesiredMovementDirection = FVector(TankInput.MovementInput.X, TankInput.MovementInput.Y, 0.0f);
		if (!DesiredMovementDirection.IsNearlyZero())
		{
			// Rotate the tank. This process deals with TankDirection directly, not the actor/RootComponent, because we don't want to affect the camera.
			FRotator MovementAngle = DesiredMovementDirection.Rotation();
			float DeltaYaw = UTankStatics::FindDeltaAngleDegrees(TankDirection->GetComponentRotation().Yaw, MovementAngle.Yaw);
			bool bReverse = false;
			if (DeltaYaw != 0.0f)
			{
				float AdjustedDeltaYaw = DeltaYaw;
				// If we're trying to go more than 90 degrees away from our current facing, reverse.
				if (AdjustedDeltaYaw < -90.0f)
				{
					bReverse = true;
					AdjustedDeltaYaw += 180.0f;
				}
				else if (AdjustedDeltaYaw > 90.0f)
				{
					bReverse = true;
					AdjustedDeltaYaw -= 180.0f;
				}

				// Adjust toward our desired angle, and stop if we've reached it.
				float MaxYawThisFrame = YawSpeed * DeltaSeconds;
				if (MaxYawThisFrame >= FMath::Abs(AdjustedDeltaYaw))
				{
					if (bReverse)
					{
						// Move backward. Use a temp variable in case we need MovementAngle to be correct later.
						FRotator FacingAngle = MovementAngle;
						FacingAngle.Yaw = MovementAngle.Yaw + 180.0f;
						TankDirection->SetWorldRotation(FacingAngle);
					}
					else
					{
						// Finish, moving forward. Facing and movement are the same angle.
						TankDirection->SetWorldRotation(MovementAngle);
					}
				}
				else
				{
					// Adjust as far as we can this frame, because we know we can't reach the goal yet.
					TankDirection->AddLocalRotation(FRotator(0.0f, FMath::Sign(AdjustedDeltaYaw) * MaxYawThisFrame, 0.0f));
				}
			}

			// Move the tank.
			{
				FVector MovementDirection = TankDirection->GetForwardVector() * (bReverse ? -1.0f : 1.0f);
				FVector Pos = GetActorLocation();
				Pos.X += MovementDirection.X * 100.0f * DeltaSeconds;
				Pos.Y += MovementDirection.Y * 100.0f * DeltaSeconds;
				SetActorLocation(Pos);
			}
		}
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveX", this, &ATank::MoveX);
	InputComponent->BindAxis("MoveY", this, &ATank::MoveY);
	InputComponent->BindAction("Fire1", EInputEvent::IE_Pressed, this, &ATank::Fire1Pressed);
	InputComponent->BindAction("Fire1", EInputEvent::IE_Released, this, &ATank::Fire1Released);
	InputComponent->BindAction("Fire2", EInputEvent::IE_Pressed, this, &ATank::Fire2Pressed);
	InputComponent->BindAction("Fire2", EInputEvent::IE_Released, this, &ATank::Fire2Released);
}

void ATank::MoveX(float AxisValue)
{
	TankInput.MoveX(AxisValue);
}

void ATank::MoveY(float AxisValue)
{
	TankInput.MoveY(AxisValue);
}

void ATank::Fire1Pressed()
{
	TankInput.Fire1(true);
}

void ATank::Fire1Released()
{
	TankInput.Fire1(false);
}

void ATank::Fire2Pressed()
{
	TankInput.Fire2(true);
}

void ATank::Fire2Released()
{
	TankInput.Fire2(false);
}
