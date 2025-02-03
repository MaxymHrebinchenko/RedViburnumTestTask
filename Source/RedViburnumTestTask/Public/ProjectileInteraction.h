// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
class AProjectileBase;
#include "ProjectileInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProjectileInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REDVIBURNUMTESTTASK_API IProjectileInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual AProjectileBase * GetProjectile() = 0;
	virtual void ReceiveDamage(float) = 0;
};
