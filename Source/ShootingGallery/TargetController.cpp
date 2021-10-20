// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetController.h"
#include "Target.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterController.h"
#include "ShootingGalleryHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

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

	// Get the player controller and sub to events
	PlayerController = Cast<AShooterController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(PlayerController)
	{
		PlayerController->OnTargetHit.BindUObject(this, &ATargetController::OnTargetWasHit);
		PlayerController->OnShotMissed.BindUObject(this, &ATargetController::OnShotWasMissed);
	}

	// Create the user interface
	if(IsValid(PlayerHUDToSpawn))
	{
		PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDToSpawn);
		if(PlayerHUD != nullptr)
		{
			PlayerHUD->AddToViewport();
			TArray<UWidget*> Widgets;
			PlayerHUD->WidgetTree->GetAllWidgets(Widgets);
		}
			
	}
		
	// Spawn targets
	SpawnTargets();

	// Set the score to 0
	Score = 0;
	// Set missed shots to 0
	MissedShots = 0;
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

void ATargetController::OnShotWasMissed()
{
	// Add to missed shots
	MissedShots++;
	// End the game if enough shots missed
	if(MaxAllowedMissedShots > 0 && MissedShots > MaxAllowedMissedShots)
	{
		GameOver();
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

float ATargetController::GetRemainingMissedShots()
{
	return float(MaxAllowedMissedShots - MissedShots);
}


void ATargetController::GameOver()
{
	UE_LOG(LogType, Warning, TEXT("Game Over!"));
}
