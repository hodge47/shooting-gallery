// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterController.h"

#include "Target.h"
#include "TargetController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AShooterController::AShooterController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the root component
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneRootComponent);

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

	// Get the TargetController - there should only be one in the scene so we will grab the first one
	TArray<AActor*> AllTargetControllerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetController::StaticClass(), AllTargetControllerActors);
	if(AllTargetControllerActors.Num() > 0)
		TargetController = Cast<ATargetController>(AllTargetControllerActors[0]);
}

// Called every frame
void AShooterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Input bound function to look up and down
void AShooterController::LookUp(float value)
{
	if(value == 0.f) return;

	FRotator NewLookPitchRotator = FRotator(value,0,0);
	SpringArm->AddRelativeRotation(NewLookPitchRotator);
}

// Input bound function to look right and left
void AShooterController::LookRight(float value)
{
	if(value == 0.f) return;

	FRotator NewLookYawRotator = FRotator(0, value, 0);
	SpringArm->AddRelativeRotation(NewLookYawRotator);
}

// Input bound function to fire bb gun
void AShooterController::Fire()
{
	// Get the viewport size so we can get the center of the screen
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Set the middle of the screen where the crosshair visuals will be
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// De-project screen to world so we know where to fire the line trace from
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	// If de-projection was successful, allow the gun to fire
	if(bScreenToWorld)
	{
		// Return if there is no TargetController or if the game is not active
		if(!IsValid(TargetController) || !TargetController->GetIsGameActive()) return;
		
		FHitResult ScreenTraceHit;
		const FVector Start = CrosshairWorldPosition;
		const FVector End = CrosshairWorldPosition + CrosshairWorldDirection * 50000.f;
		FVector LineTraceHitLocation = End;

		// Invoke line trace from the de-projected point in world space
		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
		
		// If there is a blocking hit, see if we hit a target or hit something else
		if(ScreenTraceHit.bBlockingHit)
		{
			ATarget* HitActor = Cast<ATarget>(ScreenTraceHit.GetActor());
			// If the actor hit was a target, broadcast that a target was hit. It's up to the TargetController to validate score and legal hit if it is subscribed
			if(HitActor)
			{
				// Tell the target is was hit
				HitActor->HitTarget();
				// Broadcast that a target was hit
				OnTargetHit.ExecuteIfBound(HitActor);
				// Play the shot hit sound cue
				if(ShotHitSoundCue)
					UGameplayStatics::PlaySound2D(GetWorld(), ShotHitSoundCue, 1.f, 1.f, 0.f);
			}
			// If the actor is not a target, broadcast that the shot was missed.
			else
			{
				// Raise shot missed event dispatcher
				OnShotMissed.ExecuteIfBound();
				// Play the shot missed sound cue
				if(ShotMissedSoundCue)
					UGameplayStatics::PlaySound2D(GetWorld(), ShotMissedSoundCue, 1.f, 1.f, 0.f);
			}

			// Spawn impact particles here if desired using the hit location
			LineTraceHitLocation = ScreenTraceHit.Location;
			if(ImpactParticles)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, LineTraceHitLocation);
		}
		// If there was no blocking hit, broadcast that the shot was missed
		else
		{
			// Raise shot missed event dispatcher
			OnShotMissed.ExecuteIfBound();
			// Play the shot missed sound cue
			if(ShotMissedSoundCue)
				UGameplayStatics::PlaySound2D(GetWorld(), ShotMissedSoundCue, 1.f, 1.f, 0.f);
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
	// ShooterController input
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AShooterController::Fire);
}

