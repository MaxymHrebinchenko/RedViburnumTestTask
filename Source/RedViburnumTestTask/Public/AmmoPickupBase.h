// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "AmmoPickupBase.generated.h"

/**
 * 
 */
UCLASS()
class REDVIBURNUMTESTTASK_API AAmmoPickupBase : public APickupBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 Ammo;
public:
	AAmmoPickupBase();
	virtual int32 GiveAmmo() override;
};
