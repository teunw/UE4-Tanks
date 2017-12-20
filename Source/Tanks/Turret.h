// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Tank.h"
#include "Turret.generated.h"

UCLASS()
class TANKS_API ATurret : public AActor
{
GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FORCEINLINE UArrowComponent* GetTurretDirection() { return TurretDirection; }

	/** The name of the socket at the muzzle - used for spawning missiles. */
	static const FName MuzzleSocketName;

protected:
	// Projectile to spawn when firing.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
	TSubclassOf<AActor> Projectile;

private:
	// Helpful debug tool - which way is the turret facing?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* TurretDirection;

	// Sprite for the turret.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* TurretSprite;

	// Tank that owns this turret.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	ATank* Tank;

	// Maximum rate at which the turret can turn to aim.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	float YawSpeed;

	// Time to delay between Fire1 commands.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	float Fire1Cooldown;

	// If this value is greater than the current game time, Fire1 is ignored because it has been fired too recently.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	float Fire1ReadyTime;
};
