// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePoolComponent.h"
#include "ProjectileBase.h"

// Sets default values for this component's properties
UProjectilePoolComponent::UProjectilePoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UProjectilePoolComponent::ReturnProjectile(AProjectileBase* Projectile)
{
	Pool.Enqueue(Projectile);
}

AProjectileBase* UProjectilePoolComponent::GetProjectile()
{
	AProjectileBase* projectile = nullptr;
	if (Pool.Dequeue(projectile))
	{
		return projectile;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't get projectile from pool."));
		return nullptr;
	}
}

// Called when the game starts
void UProjectilePoolComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < PoolSize; i++)
	{
		auto projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, FVector(), FRotator());
		Pool.Enqueue(projectile);
		projectile->OnProjectileExpired.BindLambda([this](AProjectileBase* projectile)
			{
				Pool.Enqueue(projectile);
			});
	}

}