// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetController.h"
#include "Target.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterController.h"

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

	// Get the player controller and sub to target hit event
	PlayerController = Cast<AShooterController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(PlayerController)
		PlayerController->OnTargetHit.AddDynamic(this, &ATargetController::OnTargetWasHit);
		
	// Spawn targets
	SpawnTargets();

	// Set the score to 0
	Score = 0;
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

void ATargetController::OnTargetWasHit(ATarget* Target)
{
	if(Target && Target->GetIsActive())
	{
		// Set the target inactive
		Target->SetTargetActive(false);
		// Add points to score
		AddToScore(Target->GetPointsValue());
		// Reset the target
		ResetTarget(Target);
	
		UE_LOG(LogType, Warning, TEXT("Score: %i"), Score);
	}
}

void ATargetController::AddToScore(int points)
{
	Score += points;
}


void ATargetController::ResetTarget(ATarget* Target)
{
	if(Target)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=]()
		{
			Target->RaiseTarget();
		}, ResetTimeSeconds, false);
	}
}

void ATargetController::RaiseTarget(ATarget* Target)
{
	Target->RaiseTarget();
}


// Called every frame
void ATargetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

