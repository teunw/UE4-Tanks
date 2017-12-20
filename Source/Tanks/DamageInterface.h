// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DamageInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TANKS_API IDamageInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void ReceiveDamage(int32 IncomingDamage) = 0;
	virtual int32 GetHealthRemaining() = 0;
};
