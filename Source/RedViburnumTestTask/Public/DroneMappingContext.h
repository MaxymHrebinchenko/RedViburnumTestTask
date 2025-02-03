// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "DroneMappingContext.generated.h"

/**
 * 
 */
UCLASS()
class REDVIBURNUMTESTTASK_API UDroneMappingContext : public UInputMappingContext
{
	GENERATED_BODY()
public:
	UDroneMappingContext();

	TObjectPtr<const UInputAction>& GetMovementAction();
	TObjectPtr<const UInputAction>& GetRotationAction();
	TObjectPtr<const UInputAction>& GetShootingAction();
	TObjectPtr<const UInputAction>& GetLookAroundAction();

protected:

	TMap<FString, TObjectPtr<const UInputAction>> ActionMap;

	UInputAction* Movement;
	UInputAction* Rotation;
	UInputAction* Shooting;
	UInputAction* LookAround;

	UInputModifierNegate* Negate;
	UInputModifierSwizzleAxis* SwizzleMovement;
	UInputModifierSwizzleAxis* SwizzleAscension;

	FEnhancedActionKeyMapping Forward;
	FEnhancedActionKeyMapping Backward;
	FEnhancedActionKeyMapping Left;
	FEnhancedActionKeyMapping Right;
	FEnhancedActionKeyMapping Ascend;
	FEnhancedActionKeyMapping Descend;
	FEnhancedActionKeyMapping TurnRight;
	FEnhancedActionKeyMapping TurnLeft;
	FEnhancedActionKeyMapping Shoot;
	FEnhancedActionKeyMapping MouseLook;
	
	
};
