// Fill out your copyright notice in the Description page of Project Settings.

class UInputMappingContext;
#include "DroneBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h" 

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include <exception>
#include <ProjectilePoolComponent.h>
#include <DroneMappingContext.h>

// Sets default values
ADroneBase::ADroneBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//Init mesh and scene components
	Drone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	RootComponent = Drone;//CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	LeftBlaster = CreateDefaultSubobject<USceneComponent>(TEXT("LeftBlaster"));
	RightBlaster = CreateDefaultSubobject<USceneComponent>(TEXT("RightBlaster"));
	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	//Attaching the components
	Drone->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName(TEXT("Drone")));

	Arm->AttachToComponent(Drone, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName(TEXT("Drone")));

	Camera->AttachToComponent(Arm,FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName(TEXT("Drone")));

	LeftBlaster->AttachToComponent(Drone, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	RightBlaster->AttachToComponent(Drone, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	
	auto component = Camera->GetAttachParent();

	//Drone's projectile properties
	ShootDelay = 0.15;
	bCanShoot = true;
	ProjectileSpeed = 2000;

	//Drone
	Speed = 500;
	YawLimit = 30;
	PitchLimit = { 10,-85 };
	CurrentBlaster = LeftBlaster;
	AlignmentDistance = 2000;
	MaxHP = 100;
	HP = MaxHP;
	Damage = 10;
	MaxAmmo = 50;
	Ammo = MaxAmmo;

	//Attach component;
	ProjectilePoolComponent = CreateDefaultSubobject<UProjectilePoolComponent>("ProjectilePool");

	//Input
	//For some reason it still null. Must create on request
	//DroneMappingContext = NewObject<UDroneMappingContext>();
}

// Called when the game starts or when spawned
void ADroneBase::BeginPlay()
{
	Super::BeginPlay();

	Drone->OnComponentBeginOverlap.AddDynamic(this, &ADroneBase::PickupOverlap);
}

// Called to bind functionality to input
void ADroneBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		if (DroneMappingContext == nullptr)
		{
			DroneMappingContext = NewObject<UDroneMappingContext>();
		}

		//Can't find another way to bind input
		EnhancedInputComponent->BindAction(DroneMappingContext->GetMapping(0).Action, ETriggerEvent::Triggered, this, &ADroneBase::Move);
		
		EnhancedInputComponent->BindAction(DroneMappingContext->GetMapping(6).Action, ETriggerEvent::Triggered, this, &ADroneBase::RotateDrone);
		
		EnhancedInputComponent->BindAction(DroneMappingContext->GetMapping(8).Action, ETriggerEvent::Triggered, this, &ADroneBase::LookAround);

		EnhancedInputComponent->BindAction(DroneMappingContext->GetMapping(9).Action, ETriggerEvent::Triggered, this, &ADroneBase::Shoot);
	}
}

void ADroneBase::PossessedBy(AController* NewController)
{
	if (NewController->IsA<APlayerController>())
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(NewController)->GetLocalPlayer()))
		{
			if (DroneMappingContext == nullptr)
			{
				DroneMappingContext = NewObject<UDroneMappingContext>();
			}

			subsystem->AddMappingContext(DroneMappingContext, 0);
		}
	}
}

AProjectileBase* ADroneBase::GetProjectile()
{
	return ProjectilePoolComponent->GetProjectile();
}

void ADroneBase::ResetShooting()
{
	bCanShoot = true;
}

void ADroneBase::GameOver()
{
	UGameplayStatics::SetGamePaused(this,true);
}

void ADroneBase::PickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickupInteraction* interface = Cast<IPickupInteraction>(OtherActor))
	{
		Heal(interface->Heal());
		ReceiveAmmo(interface->GiveAmmo());
		OtherActor->Destroy();
	}
}

void ADroneBase::ReceiveDamage(float damage)
{
	HP -= damage;
	if (HP <= 0)
		GameOver();
}

void ADroneBase::Heal(float hp)
{
	HP += hp;
	if (HP > MaxHP)
		HP = MaxHP;
}

float ADroneBase::GetHealth()
{
	return HP;
}

int32 ADroneBase::GetAmmo()
{
	return Ammo;
}

void ADroneBase::ReceiveAmmo(int32 ammo)
{
	Ammo += ammo;
	if (Ammo > MaxAmmo)
		Ammo = MaxAmmo;
}

void ADroneBase::Move(const FInputActionValue& Value)
{
	FVector Vector = Value.Get<FVector>();
	float Delta = UGameplayStatics::GetWorldDeltaSeconds(this);
	Vector *= Speed*Delta;
	
	FVector Forward = GetActorForwardVector();
	Forward.Z = 0.f;
	Forward.Normalize();
	
	FVector Right = GetActorRightVector();
	Right.Z = 0.f;
	Right.Normalize();

	FVector Direction = FVector::ZeroVector;
	Direction += Vector.X * Forward;
	Direction += Vector.Y * Right;
	Direction += Vector.Z/4 * FVector::UpVector;

	FHitResult MoveSweep;

	AddActorWorldOffset(Direction,true,&MoveSweep);

	if (!MoveSweep.bBlockingHit)
	{
		AddActorWorldOffset(Direction);
	}
}

void ADroneBase::LookAround(const FInputActionValue& Value)
{
	auto DeltaMouse = Value.Get<FVector2D>();
	float RotationSensitivity = 1.5f;

	float Pitch = DeltaMouse.Y * RotationSensitivity;
	float Yaw = DeltaMouse.X * RotationSensitivity;

	FRotator Rotator = Camera->GetRelativeRotation();
	FRotator Result = Rotator + FRotator(Pitch, Yaw, 0);

	if (Rotator.Pitch + Pitch > PitchLimit.X)
		Result.Pitch = PitchLimit.X;
	
	if (Rotator.Pitch + Pitch < PitchLimit.Y)
		Result.Pitch = PitchLimit.Y;

	if (Rotator.Yaw + Yaw > YawLimit)
		Result.Yaw = YawLimit;
	
	if (Rotator.Yaw + Yaw < -YawLimit)
		Result.Yaw = -YawLimit;

	Camera->SetRelativeRotation(Result);
}

void ADroneBase::RotateDrone(const FInputActionValue& Value)
{
	float Rotation = Value.Get<float>();
	float RotationSensitivity = 2.f;
	Rotation *= RotationSensitivity;

	FRotator Delta(0.f, Rotation, 0.f);
	AddActorWorldRotation(Delta);
}

void ADroneBase::Shoot(const FInputActionValue& Value)
{
	if (bCanShoot && Ammo > 0)
	{
		if (AProjectileBase* Projectile = GetProjectile())
		{
			FVector Forward = UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation());
			if (!Forward.IsNormalized())
				Forward.Normalize();

			Forward *= AlignmentDistance;

			FVector Start = Camera->GetComponentLocation();
			FVector End = Start + Forward;

			FHitResult Hit;
			APawn::GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

			Start = CurrentBlaster->GetComponentLocation();
			if (Hit.bBlockingHit)
				End = Hit.Location;

			Projectile->SetupVectors(Start, End - Start);

			bCanShoot = false;
			GetWorldTimerManager().SetTimer(DelayTimer, this, &ADroneBase::ResetShooting, ShootDelay, false);

			SwitchBlaster();
			Ammo--;
			Projectile->Enable(ProjectileSpeed, Damage);
		}
	}	
}

void ADroneBase::SwitchBlaster()
{
	if (CurrentBlaster == LeftBlaster)
	{
		CurrentBlaster = RightBlaster;
		return;
	}
	CurrentBlaster = LeftBlaster;
}