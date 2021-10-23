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
	ShooterController = Cast<AShooterController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(ShooterController)
	{
		ShooterController->OnTargetHit.BindUObject(this, &ATargetController::OnTargetWasHit);
		ShooterController->OnShotMissed.BindUObject(this, &ATargetController::OnShotWasMissed);
	}

	// Create the user interface
	if(IsValid(PlayerWidgetToSpawn))
	{
		PlayerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetToSpawn);
		if(PlayerWidget != nullptr)
		{
			PlayerWidget->AddToViewport();
		}
	}
		
	// Spawn targets
	SpawnTargets();

	// Set the score to 0
	Score = 0;
	// Set missed shots to 0
	MissedShots = 0;

	// Set the game state to not active
	bIsGameActive = false;
	// Start the game after x seconds
	GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &ATargetController::StartGame, 3.5f, false);
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

void ATargetController::StartGame()
{
	bIsGameActive = true;
	// Start the game timer
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ATargetController::GameOver, GameTimeInSeconds, false);
}

void ATargetController::OnTargetWasHit(ATarget* Target)
{
	if(Target && Target->GetIsActive() && bIsGameActive)
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

int ATargetController::GetRemainingMissedShots()
{
	return int(MaxAllowedMissedShots - MissedShots);
}

// Modified function from https://mylittledevblog.wordpress.com/2018/02/15/ue4-add-leading-zeroes/
FString ATargetController::GetPrettyCurrentScore()
{
	char Buffer[100];
	int RespCode;
	RespCode = snprintf(Buffer, 100, "%0*d", 7, Score);
	return FString(ANSI_TO_TCHAR(Buffer));
}



void ATargetController::GameOver()
{
	// Set the game to inactive so the player cannot score anymore
	bIsGameActive = false;

	// End any timers that we might have going
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// Display the game over widget
	if(IsValid(GameOverWidgetToSpawn))
	{
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetToSpawn);
		if(GameOverWidget != nullptr)
		{
			GameOverWidget->AddToViewport();
			GameOverWidget->SetFocus();
			// Make sure to enter UI input mode
			auto PlayerController = GetWorld()->GetFirstPlayerController();
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
			
		}
	}
}

bool ATargetController::GetIsGameActive()
{
	return bIsGameActive;
}


int ATargetController::GetTimeLeft()
{
	int TimeLeft = 0;
	
	if(!bIsGameActive)
		TimeLeft = int(GetWorld()->GetTimerManager().GetTimerRemaining(StartGameTimerHandle));
	else
		TimeLeft = int(GetWorld()->GetTimerManager().GetTimerRemaining(GameTimerHandle));

	return TimeLeft;
}

void ATargetController::RestartGame()
{
	// Set the score to 0
	Score = 0;
	// Set missed shots to 0
	MissedShots = 0;

	// Set the game state to not active
	bIsGameActive = false;
	// Start the game after x seconds
	GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &ATargetController::StartGame, 3.5f, false);
}

