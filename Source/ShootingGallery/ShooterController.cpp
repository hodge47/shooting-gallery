// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterController.h"

#include "Target.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShooterController::AShooterController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the root component
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneRootComponent);

	// Create the mesh component
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));

	// Create the spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create the camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, SpringArm->SocketName);

}

// Called when the game starts or when spawned
void AShooterController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterController::LookUp(float value)
{
	if(value == 0.f) return;

	FRotator NewLookPitchRotator = FRotator(value,0,0);
	SpringArm->AddRelativeRotation(NewLookPitchRotator);
}

void AShooterController::LookRight(float value)
{
	if(value == 0.f) return;

	FRotator NewLookYawRotator = FRotator(0, value, 0);
	SpringArm->AddRelativeRotation(NewLookYawRotator);
}

void AShooterController::Fire()
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if(bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start = CrosshairWorldPosition;
		const FVector End = CrosshairWorldPosition + CrosshairWorldDirection * 50000.f;

		FVector BeamEndPoint = End;
		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
		if(ScreenTraceHit.bBlockingHit)
		{
			BeamEndPoint = ScreenTraceHit.Location;
			ATarget* HitActor = Cast<ATarget>(ScreenTraceHit.GetActor());
			if(HitActor)
			{
				HitActor->HitTarget();
				// Broadcast that a target was hit
				OnTargetHit.Broadcast(HitActor);
			}
			else
			{
				// Raise shot missed event dispatcher
				OnShotMissed.Broadcast();
			}
		}
		else
		{
			// Raise shot missed event dispatcher
			OnShotMissed.Broadcast();
		}
	}
}

// Called to bind functionality to input
void AShooterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Camera input
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterController::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AShooterController::LookRight);
	// Controller input
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AShooterController::Fire);
}

