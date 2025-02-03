// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REDVIBURNUMTESTTASK_API IPickupInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float Heal() { return 0; };
	virtual int32 GiveAmmo() { return 0; };
};
