// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetAnimInstance.h"
#include "Target.h"

void UTargetAnimInstance::NativeInitializeAnimation()
{
	Target = Cast<ATarget>(TryGetPawnOwner());
	bIsTargetActive = true;
	bIsTargetFalling = false;
	bIsTargetRaising = false;
}

void UTargetAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	// Get a target reference if one is not set
	if(Target == nullptr)
	{
		Target = Cast<ATarget>(TryGetPawnOwner());
	}
}

// Plays the target falling animation
void UTargetAnimInstance::InitTargetFall()
{
	bIsTargetActive = false;
	bIsTargetFalling = true;
	bIsTargetRaising = false;
}

// Plays the target raising animation
void UTargetAnimInstance::InitTargetRaise()
{
	bIsTargetActive = true;
	bIsTargetFalling = false;
	bIsTargetRaising = true;
}
