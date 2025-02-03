// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectilePoolComponent.generated.h"

class AProjectileBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REDVIBURNUMTESTTASK_API UProjectilePoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectilePoolComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AProjectileBase> ProjectileClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PoolSize;

	void ReturnProjectile(AProjectileBase* Projectile);
	AProjectileBase* GetProjectile();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TQueue<AProjectileBase*> Pool;		
};
