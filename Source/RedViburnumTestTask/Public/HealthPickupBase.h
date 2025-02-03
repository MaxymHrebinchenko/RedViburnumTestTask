// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "HealthPickupBase.generated.h"

/**
 * 
 */
UCLASS()
class REDVIBURNUMTESTTASK_API AHealthPickupBase : public APickupBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HP;
public:
	AHealthPickupBase();
	virtual float Heal() override;
};
