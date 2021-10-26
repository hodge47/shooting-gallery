// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "Components/SkeletalMeshComponent.h"
#include "TargetAnimInstance.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the target mesh
	TargetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Target Mesh"));
	RootComponent = TargetMesh;
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();

	// Make the target active
	bIsActive = true;

	// Get the target's animation instance
	if(TargetMesh)
	{
		TargetAnimInstance = Cast<UTargetAnimInstance>(TargetMesh->GetAnimInstance());
	}
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called by the target controller to tell this target to go inactive and play the falling animation
void ATarget::HitTarget()
{
	if(TargetAnimInstance && bIsActive)
	{
		// Trigger the fall animation
		TargetAnimInstance->InitTargetFall();
	}
		
}

// Called by the target controller to tell this target to go active and play the raising animation
void ATarget::RaiseTarget()
{
	if(TargetAnimInstance && !bIsActive)
	{
		// Raise the target up
		TargetAnimInstance->InitTargetRaise();
		// Set the target active
		bIsActive = true;
	}
}

// Returns the points value of this target
int ATarget::GetPointsValue()
{
	return Points;
}

// Set the target active or inactive based on the supplied argument
void ATarget::SetTargetActive(bool bTargetIsActive)
{
	bIsActive = bTargetIsActive;
}

// Returns the active state of the target
bool ATarget::GetIsActive()
{
	return bIsActive;
}

