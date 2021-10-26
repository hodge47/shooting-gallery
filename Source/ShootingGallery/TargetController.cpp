// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetController.h"

#include "HighScoreSaveGame.h"
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
	// Get the high score
	HighScore = 0;
	LoadedDelegate.BindUObject(this, &ATargetController::OnLoadedHighScore);
	UGameplayStatics::AsyncLoadGameFromSlot(TEXT("high_score"), 0, LoadedDelegate);

	// Set the game state to not active
	bIsGameActive = false;
	// Start the game after x seconds
	GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &ATargetController::StartGame, 3.5f, false);
}

// Called every frame
void ATargetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Subscribes to the high score load delegate
void ATargetController::OnLoadedHighScore(const FString& SlotName, const int32 UserIndex, class USaveGame* LoadedGameData)
{
	const auto HighScoreSaveGameData = Cast<UHighScoreSaveGame>(LoadedGameData);
	if(HighScoreSaveGameData && HighScoreSaveGameData->HighScore > 0)
	{
		HighScore = HighScoreSaveGameData->HighScore;
		UE_LOG(LogType, Warning, TEXT("High score: %i"), HighScore);
	}
}


// Spawns the targets on the grid
void ATargetController::SpawnTargets()
{
	// Build grid specifications
	const int GridX = (int)TargetGridSize.X;
	const int GridY = (int)TargetGridSize.Y;
	const float SpacingY = TargetGridSpacing.X;
	const float SpacingZ = TargetGridSpacing.Y;
	float OffsetY = GetActorLocation().Y;
	float OffsetZ = GetActorLocation().Z;

	// Continue if the target actor to spawn is valid
	if(TargetActor)
	{
		FActorSpawnParameters SpawnParameters;
		for(int i = 0; i < GridX; i++)
		{
			for (int j = 0; j < GridY; j++)
			{
				// Spawn a target at a specific spacing
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

// Starts the game
void ATargetController::StartGame()
{
	// Set the game active
	bIsGameActive = true;
	// Start the game timer
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ATargetController::GameOver, GameTimeInSeconds, false);
}

// Subscribes to ShooterController hit delegate
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

// Subscribes to ShooterController shot missed delegate
void ATargetController::OnShotWasMissed()
{
	// Add to missed shots
	MissedShots++;
	// End the game if enough shots missed
	if(MaxAllowedMissedShots > 0 && MissedShots > MaxAllowedMissedShots - 1)
	{
		GameOver();
	}
}

// Adds points to the current player score
void ATargetController::AddToScore(int points)
{
	// Add to the score by points passed in
	Score += points;

	// Save the high score if the current score is bigger than the latest high score
	if(Score > HighScore)
	{
		HighScore = Score;
		if(UHighScoreSaveGame* SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass())))
		{
			// Save the high score
			SaveGameInstance->HighScore = HighScore;
			// Start async save process
			UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, TEXT("high_score"), 0);
		}
	}
}

// Waits x seconds to raise the target
void ATargetController::ResetTarget(ATarget* Target)
{
	if(Target)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=]()
		{
			// Raise the target
			Target->RaiseTarget();
		}, ResetTimeSeconds, false);
	}
}

// Calls the RaiseTarget function on any target in the target list
void ATargetController::RaiseTarget(ATarget* Target)
{
	Target->RaiseTarget();
}

// Gets how many missed shots the player has left
int ATargetController::GetRemainingMissedShots()
{
	return int(MaxAllowedMissedShots - MissedShots);
}

/*
// Gets a formatted version of an inputted integer: 247 -> 0000247
// Modified function from https://mylittledevblog.wordpress.com/2018/02/15/ue4-add-leading-zeroes/
*/
FString ATargetController::GetPrettyInteger(int InputInteger)
{
	char Buffer[100];
	snprintf(Buffer, 100, "%0*d", 7, InputInteger);
	return FString(ANSI_TO_TCHAR(Buffer));
}

// Ends the game and displays the GameOver widget
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
			// Add the game over widget to the viewport and focus it
			GameOverWidget->AddToViewport();
			GameOverWidget->SetFocus();
			// Make sure to enter UI input mode
			auto PlayerController = GetWorld()->GetFirstPlayerController();
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
			
		}
	}
}

// Gets the active state of the game
bool ATargetController::GetIsGameActive()
{
	return bIsGameActive;
}

// Gets the time left in the active game
int ATargetController::GetTimeLeft()
{
	int TimeLeft = 0;

	// Just supply the time based on the current game state - this can probably be made more intuitive
	if(!bIsGameActive)
		TimeLeft = int(GetWorld()->GetTimerManager().GetTimerRemaining(StartGameTimerHandle));
	else
		TimeLeft = int(GetWorld()->GetTimerManager().GetTimerRemaining(GameTimerHandle));

	return TimeLeft;
}

// Gets the player's current score in the active game
int ATargetController::GetCurrentScore()
{
	return Score;
}

// Gets the player's all-time high score
int ATargetController::GetHighScore()
{
	return HighScore;
}

// Resets the game state/score/time and restarts the game
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