// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "DroneBase.h"
#include "TurretBase.h"
#include <exception>
// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	LifeDuration = 2;
	Speed = 5000;
	Radius = 20.f;

	RootComponent = Projectile;
	RootComponent->SetAbsolute(false, false, true);
	RootComponent->SetWorldScale3D(FVector(0.01) * Radius);

	Disable();
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	Projectile->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::ProjectileOverlap);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Delta = UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorWorldOffset(Speed * Delta * Direction);
}

void AProjectileBase::ProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IProjectileInteraction* interface = Cast<IProjectileInteraction>(OtherActor);
	if (interface && OtherActor != this->GetOwner())
	{
		interface->ReceiveDamage(Damage);
	}

	Disable();
}

void AProjectileBase::Disable()
{
	if (LifeTimer.IsValid())
		LifeTimer.Invalidate();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	OnProjectileExpired.ExecuteIfBound(this);
}

void AProjectileBase::Enable(float speed, float damage)
{
	Damage = damage;
	Speed = speed;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	
	GetWorldTimerManager().SetTimer(LifeTimer, this, &AProjectileBase::Disable, LifeDuration, false);
}

void AProjectileBase::SetupVectors(FVector Start, FVector Dir)
{
	SetActorLocation(Start);
	Direction = Dir;
	if (!Direction.IsNormalized())
		Direction.Normalize();
}


