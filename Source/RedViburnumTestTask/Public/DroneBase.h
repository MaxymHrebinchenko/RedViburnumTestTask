// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"

#include "ProjectileBase.h"
#include "ProjectileInteraction.h"
#include "PickupBase.h"

#include "GameFramework/Pawn.h"
#include "DroneBase.generated.h"

class UInputMappingContext;
class UProjectilePoolComponent;
class UDroneMappingContext;

UCLASS()
class REDVIBURNUMTESTTASK_API ADroneBase : public APawn, public IProjectileInteraction
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADroneBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Viewport
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	UStaticMeshComponent* Drone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	USceneComponent* LeftBlaster;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	USceneComponent* RightBlaster;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	USpringArmComponent* Arm;

	USceneComponent* CurrentBlaster;

	//Drone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float YawLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FVector2f PitchLimit;
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MaxHP;
	float Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float Damage;


	//Projectiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
	float AlignmentDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
	float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
	float ShootDelay;
	
	FTimerHandle DelayTimer;
	bool bCanShoot;

	//Components
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProjectilePoolComponent* ProjectilePoolComponent;

	UDroneMappingContext* DroneMappingContext;

	//Input Actions
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void RotateDrone(const FInputActionValue& Value);

	virtual AProjectileBase* GetProjectile() override;
	void SwitchBlaster();
	void ResetShooting();
	void GameOver();

	virtual void PossessedBy(AController* NewController) override;

	//
	UFUNCTION()
	void PickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintCallable)
	int32 GetAmmo();
public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void ReceiveDamage(float) override;
	virtual void Heal(float);
	virtual void ReceiveAmmo(int32);
};
