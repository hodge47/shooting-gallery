// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetAnimInstance.h"
#include "Target.h"

void UTargetAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(Target == nullptr)
	{
		Target = Cast<ATarget>(TryGetPawnOwner());
	}
	if(Target)
	{
		
	}
}

void UTargetAnimInstance::InitTargetFall()
{
	bIsTargetActive = false;
	bIsTargetFalling = true;
	bIsTargetRaising = false;
}

void UTargetAnimInstance::InitTargetRaise()
{
	bIsTargetActive = true;
	bIsTargetFalling = false;
	bIsTargetRaising = true;
}


void UTargetAnimInstance::NativeInitializeAnimation()
{
	Target = Cast<ATarget>(TryGetPawnOwner());
	bIsTargetActive = true;
	bIsTargetFalling = false;
	bIsTargetRaising = false;
}
