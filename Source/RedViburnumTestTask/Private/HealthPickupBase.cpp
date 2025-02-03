// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickupBase.h"

AHealthPickupBase::AHealthPickupBase()
{
	HP = 25;
}

float AHealthPickupBase::Heal()
{
	return HP;
}
