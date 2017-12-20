// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"
#include "DamageInterface.h"
#include "Camera/CameraShake.h"
#include "Zombie.generated.h"

class ATank;

UCLASS()
class TANKS_API AZombie : public APawn, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AZombie();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Set this zombie's target. The base version of this function will handle updating TargetActor and TargetTank appropriately. Input parameter can be NULL.
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTarget(AActor* NewTarget);

	// Return the target Actor. Returning NULL indicates no target.
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTarget();

	// Return the target Actor as a Tank, if possible. Returning NULL indicates no target, or that the target is not a Tank.
	UFUNCTION(BlueprintCallable, Category = "AI")
	ATank* GetTargetAsTank();

	// Zombies will call this on Tick.
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	void ZombieAI(float DeltaSeconds);
	virtual void ZombieAI_Implementation(float DeltaSeconds);

	// This function asks the zombie if it is in position to attack its current target. It does not actually command the zombie to attack.
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	bool ZombieAIShouldAttack();
	virtual bool ZombieAIShouldAttack_Implementation();

	// This can be used to animate walking.
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void ZombieWalk(float DeltaSeconds, FVector DistanceWalkedThisFrame);

	// This can be used to start attack animations.
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void ZombieAttack(float DeltaSeconds);

	//~
	//~ New Zombie Input
	//~

	/** Add rotation equal to the yaw (in degrees) provided. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
	virtual void AddRotationInput(float DeltaYawDegrees);

	/** Get rotation input. Returns pre-clear value. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual float GetRotationInput();

	/** Get (and clear) rotation input. Returns pre-clear value. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual float ConsumeRotationInput();

	/** Press the attack button. The pawn will know what to do with this. No arguments because it's a bool and this sets it to true. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
	virtual void AddAttackInput();

	/** Get the status of the attack button. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual bool GetAttackInput();

	/** Get (and clear) the status of the attack button. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual bool ConsumeAttackInput();

	//~ Begin IDamageInterface
	virtual void ReceiveDamage(int32 IncomingDamage) override;
	virtual int32 GetHealthRemaining() override;
	//~ End IDamageInterface

private:
	/* The actor we're targeting. Will be NULL if there is no target. */
	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	AActor* TargetActor;

	/* The actor we're targeting, cast to a Tank and cached. Will be NULL if no target or if the target actor is not a Tank. */
	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	ATank* TargetTank;

protected:
	/** Camera effect, if any, to play when the player is hit by this Zombie. */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> HitShake;

	/** Current health value. Might be fun to have different values for different attack types, e.g. fire, explosions, etc. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	int32 Health;

	/** Sight distance (when no target is present) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float SightDistance;

	/** Sight half-angle in degrees (when no target is present) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float SightAngle;

	/** Max yaw speed per second in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float YawSpeed;

	/** Speed when wandering around, or when facing away from target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float WalkSpeed;

	/** Speed when rushing toward target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float RunSpeed;

	/** Attack distance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float AttackDistance;

	/** Half-angle (in degrees) for our attack. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float AttackAngle;

	/** Minimum time between attacks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float AttackCooldown;

	/** Game time, in seconds, when the Zombie will be allowed to attack again. */
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadWrite, Category = "Zombie")
	float AttackAvailableTime;

private:
	// Current rotation input.
	float YawInput;

	// Current attack button status.
	uint32 bAttackInput : 1;
};
