// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "TargetController.generated.h"

UCLASS()
class SHOOTINGGALLERY_API ATargetController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// The target actor to spawn (blueprint)
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	TSubclassOf<class ATarget> TargetActor;
	// The grid that the spawned targets should follow
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector2D TargetGridSize = FVector2D(3.f, 4.f);
	// The spacing of the targets in the grid
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector2D TargetGridSpacing = FVector2D(50.f, 100.f);
	// How fast the targets should reset after a valid hit
	UPROPERTY(EditDefaultsOnly, Category="Target Resetting")
	float ResetTimeSeconds = 2.f;
	// The maximum number of shots that a player can make during the game
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Settings")
	int MaxAllowedMissedShots = 3;
	// The time in seconds that the game will run
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gameplay Settings")
	int GameTimeInSeconds = 60;

private:
	// A reference to the shooter controller the player is controlling
	UPROPERTY()
	class AShooterController* ShooterController;
	// The player HUD widget to be spawned
	UPROPERTY(EditDefaultsOnly, Category="User Interface", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PlayerWidgetToSpawn;
	// A reference to the player widget that is spawned
	UPROPERTY()
	class UUserWidget* PlayerWidget;
	// The game over HUD widget to spawn
	UPROPERTY(EditDefaultsOnly, Category="User Interface", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameOverWidgetToSpawn;
	// A reference to the game over widget that is spawned
	UPROPERTY()
	class UUserWidget* GameOverWidget;

	// A collection of spawned target actore
	TArray<ATarget*> SpawnedTargets;
	// How many missed shots the player has made
	int MissedShots;
	// The player's current score
	int Score;
	// The player's all-time high score retrieved from a save game object
	int HighScore;

	// Start game and game time timer handles
	FTimerHandle StartGameTimerHandle;
	FTimerHandle GameTimerHandle;

	// The active state of the game
	bool bIsGameActive;

	// Delegate needed to broadcast that a save game was or was not loaded
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;

public:
	// Gets how many missed shots the player has left
	UFUNCTION(BlueprintCallable)
	int GetRemainingMissedShots();
	// Gets the active state of the game
	UFUNCTION(BlueprintCallable)
	bool GetIsGameActive();
	// Gets the time left in the active game
	UFUNCTION(BlueprintCallable)
	int GetTimeLeft();
	// Gets the player's current score in the active game
	UFUNCTION(BlueprintCallable)
	int GetCurrentScore();
	// Gets the player's all-time high score
	UFUNCTION(BlueprintCallable)
	int GetHighScore();
	// Resets the game state/score/time and restarts the game
	UFUNCTION(BlueprintCallable)
	void RestartGame();
	// Gets a formatted version of an inputted integer: 247 -> 0000247
	UFUNCTION(BlueprintCallable)
	FString GetPrettyInteger(int InputInteger);
	
private:
	// Subscribes to the high score load delegate
	void OnLoadedHighScore(const FString& SlotName, const int32 UserIndex, class USaveGame* LoadedGameData);
	// Spawns the targets on the grid
	void SpawnTargets();
	// Starts the game
	void StartGame();
	// Subscribes to ShooterController hit delegate
	void OnTargetWasHit(ATarget* Target);
	// Subscribes to ShooterController shot missed delegate
	void OnShotWasMissed();
	// Adds points to the current player score
	void AddToScore(int points);
	// Waits x seconds to raise the target
	void ResetTarget(ATarget* Target);
	// Calls the RaiseTarget function on any target in the target list
	void RaiseTarget(ATarget* Target);
	// Ends the game and displays the GameOver widget
	void GameOver();
};
