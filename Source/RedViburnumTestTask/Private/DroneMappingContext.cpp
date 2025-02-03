// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneMappingContext.h"

UDroneMappingContext::UDroneMappingContext()
{
	Movement = NewObject<UInputAction>();
	Movement->ValueType = EInputActionValueType::Axis3D;

	LookAround = NewObject<UInputAction>();
	LookAround->ValueType = EInputActionValueType::Axis2D;

	Rotation = NewObject<UInputAction>();
	Rotation->ValueType = EInputActionValueType::Axis1D;

	Shooting = NewObject<UInputAction>();
	Shooting->ValueType = EInputActionValueType::Boolean;

	SwizzleMovement = NewObject<UInputModifierSwizzleAxis>();
	SwizzleMovement->Order = EInputAxisSwizzle::YXZ;
	
	SwizzleAscension = NewObject<UInputModifierSwizzleAxis>();
	SwizzleAscension->Order = EInputAxisSwizzle::ZYX;

	Negate = NewObject<UInputModifierNegate>();

	Forward = FEnhancedActionKeyMapping(Movement,EKeys::W);
	Mappings.Add(Forward);

	Backward = FEnhancedActionKeyMapping(Movement, EKeys::S);
	Backward.Modifiers.Add(Negate);
	Mappings.Add(Backward);

	Left = FEnhancedActionKeyMapping(Movement, EKeys::A);
	Left.Modifiers.Add(Negate);
	Left.Modifiers.Add(SwizzleMovement);
	Mappings.Add(Left);

	Right = FEnhancedActionKeyMapping(Movement, EKeys::D);
	Right.Modifiers.Add(SwizzleMovement);
	Mappings.Add(Right);

	Ascend = FEnhancedActionKeyMapping(Movement, EKeys::SpaceBar);
	Ascend.Modifiers.Add(SwizzleAscension);
	Mappings.Add(Ascend);

	Descend = FEnhancedActionKeyMapping(Movement, EKeys::LeftShift);
	Descend.Modifiers.Add(SwizzleAscension);
	Descend.Modifiers.Add(Negate);
	Mappings.Add(Descend);

	TurnLeft = FEnhancedActionKeyMapping(Rotation, EKeys::Q);
	TurnLeft.Modifiers.Add(Negate);
	Mappings.Add(TurnLeft);

	TurnRight = FEnhancedActionKeyMapping(Rotation, EKeys::E);
	Mappings.Add(TurnRight);

	MouseLook = FEnhancedActionKeyMapping(LookAround, EKeys::Mouse2D);
	Mappings.Add(MouseLook);

	Shoot = FEnhancedActionKeyMapping(Shooting,EKeys::LeftMouseButton);
	Mappings.Add(Shoot);

	ActionMap.Add("Movement", Forward.Action);
	ActionMap.Add("Rotation", TurnLeft.Action);
	ActionMap.Add("LookAround", MouseLook.Action);
	ActionMap.Add("Shooting", Shoot.Action);
}

TObjectPtr<const UInputAction>& UDroneMappingContext::GetMovementAction()
{
	//return GetMapping(0).Action;
	return *ActionMap.Find("Movement");
}

TObjectPtr<const UInputAction>& UDroneMappingContext::GetRotationAction()
{
	return *ActionMap.Find("Rotation");
}

TObjectPtr<const UInputAction>& UDroneMappingContext::GetShootingAction()
{
	return *ActionMap.Find("Shooting");
}

TObjectPtr<const UInputAction>& UDroneMappingContext::GetLookAroundAction()
{
	return *ActionMap.Find("LookAround");
}
