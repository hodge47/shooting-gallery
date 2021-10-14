// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetController.h"
#include "Target.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATargetController::ATargetController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATargetController::BeginPlay()
{
	Super::BeginPlay();

	// Spawn targets
	SpawnTargets();
}

void ATargetController::SpawnTargets()
{
	const int GridX = (int)TargetGridSize.X;
	const int GridY = (int)TargetGridSize.Y;
	const float SpacingY = TargetGridSpacing.X;
	const float SpacingZ = TargetGridSpacing.Y;
	float OffsetY = GetActorLocation().Y;
	float OffsetZ = GetActorLocation().Z;
	
	if(TargetActor)
	{
		FActorSpawnParameters SpawnParameters;
		for(int i = 0; i < GridX; i++)
		{
			for (int j = 0; j < GridY; j++)
			{
				ATarget* SpawnedTarget = GetWorld()->SpawnActor<ATarget>(TargetActor, GetActorLocation(), GetActorRotation(), SpawnParameters);
				SpawnedTarget->SetActorLocation(FVector(GetActorLocation().X, OffsetY, OffsetZ));
				SpawnedTargets.Push(SpawnedTarget);

				OffsetY += SpacingY;
				
			}
			OffsetY = GetActorLocation().Y;
			OffsetZ += SpacingZ;
		}
	}
}


// Called every frame
void ATargetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

