// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickupBase.h"

AAmmoPickupBase::AAmmoPickupBase()
{
	Ammo = 15;
}

int32 AAmmoPickupBase::GiveAmmo()
{
	return Ammo;
}
