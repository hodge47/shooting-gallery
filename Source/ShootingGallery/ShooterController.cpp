// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AShooterController::AShooterController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the mesh component
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	RootComponent = GunMesh;

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



// Called to bind functionality to input
void AShooterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &AShooterController::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AShooterController::LookRight);
}

