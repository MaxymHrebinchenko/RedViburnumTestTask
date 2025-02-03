// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"

#include "ProjectileBase.generated.h"

DECLARE_DELEGATE_OneParam(FOnProjectileExpired, AProjectileBase*);

UCLASS()
class REDVIBURNUMTESTTASK_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	FOnProjectileExpired OnProjectileExpired;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileProperties")
	UStaticMeshComponent* Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileProperties")
	float Radius;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "ProjectileProperties")
	float LifeDuration; 
	float Speed;
	float Damage;

	FVector Direction;
	FTimerHandle LifeTimer;
	

	void ReturnProjectile();
	UFUNCTION()
	void ProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Disable();
	void Enable(float speed,float damage);
	void SetupVectors(FVector Start, FVector Dir);
};