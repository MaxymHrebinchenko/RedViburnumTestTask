// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"

#include "ProjectileBase.h"
#include "ProjectileInteraction.h"

#include "TurretBase.generated.h"

class UProjectilePoolComponent;
class USphereComponent;

UCLASS()
class REDVIBURNUMTESTTASK_API ATurretBase : public APawn, public IProjectileInteraction
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurretBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	USceneComponent* MuzzleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UStaticMeshComponent* Core;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UStaticMeshComponent* Tower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UStaticMeshComponent* Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float Damage;
	APawn* PlayerPawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float Firerate;
	FTimerHandle FirerateTimer;
	bool bCanShoot;

	void Shoot();
	void ResetShoot();
	virtual AProjectileBase* GetProjectile() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProjectilePoolComponent* ProjectilePoolComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ReceiveDamage(float) override;
};
