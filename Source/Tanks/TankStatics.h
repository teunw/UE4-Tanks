// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TankStatics.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UTankStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Calculate angle difference in the range [-180, 180]. */
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float FindDeltaAngleDegrees(float A1, float A2);

	/** Finds the angle between two 2D points. */
	UFUNCTION(BlueprintPure, Category = "Math|Rotator", meta = (Keywords = "rotation rotate"))
	static bool FindLookAtAngle2D(const FVector2D& Start, const FVector2D& Target, float& Angle);
};
