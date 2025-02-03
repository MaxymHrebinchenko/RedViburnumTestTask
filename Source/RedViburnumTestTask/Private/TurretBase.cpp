// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBase.h"
#include "ProjectileBase.h"
#include <ProjectilePoolComponent.h>
#include <Components/SphereComponent.h>

// Sets default values
ATurretBase::ATurretBase() :
	ProjectileSpeed(2000), Distance(1000), HP(100), Damage(10)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Core = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
	Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Shoot position"));
	Core->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Tower->AttachToComponent(Core, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Muzzle->AttachToComponent(Tower, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	MuzzleComponent->AttachToComponent(Muzzle, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	
	bCanShoot = true;
	Firerate = 0.5;
	ProjectilePoolComponent = CreateDefaultSubobject<UProjectilePoolComponent>(FName("ProjectilePool"));
}

// Called when the game starts or when spawned
void ATurretBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ATurretBase::Shoot()
{
	if (AProjectileBase* Projectile = GetProjectile())
	{
		FVector Start = MuzzleComponent->GetComponentLocation();
		FVector End = PlayerPawn->GetActorLocation();
		
		Projectile->SetupVectors(Start, End - Start);
	
		Projectile->Enable(ProjectileSpeed,Damage);
	}
}

void ATurretBase::ResetShoot()
{
	bCanShoot = true;
}

// Called every frame
void ATurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	auto Location = PlayerPawn->GetActorLocation();
	FVector Direction = Location - RootComponent->GetComponentLocation();
	if (Direction.Length() <= Distance)
	{
		FRotator Rotation = Direction.ToOrientationRotator();
		Tower->SetWorldRotation(Rotation);
		if (bCanShoot)
		{
			Shoot();
			bCanShoot = false;
			GetWorldTimerManager().SetTimer(FirerateTimer, this, &ATurretBase::ResetShoot, Firerate, false);
			return;
		}
	}
	//Tower->SetRelativeRotation({0,0,0});
}

// Called to bind functionality to input
void ATurretBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

AProjectileBase* ATurretBase::GetProjectile()
{
	return ProjectilePoolComponent->GetProjectile();
}

void ATurretBase::ReceiveDamage(float damage)
{
	HP -= damage;
	if (HP <= 0)
	{
		SetActorTickEnabled(false);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}
