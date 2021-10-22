// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	TSubclassOf<class ATarget> TargetActor;
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector2D TargetGridSize = FVector2D(3.f, 4.f);
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector2D TargetGridSpacing = FVector2D(50.f, 100.f);
	UPROPERTY(EditDefaultsOnly, Category="Target Resetting")
	float ResetTimeSeconds = 2.f;
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Settings")
	int MaxAllowedMissedShots = 3;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gameplay Settings")
	int GameTimeInSeconds = 60;

private:
	UPROPERTY()
	class AShooterController* PlayerController;
	UPROPERTY(EditDefaultsOnly, Category="User Interface", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PlayerHUDToSpawn;
	UPROPERTY()
	class UUserWidget* PlayerHUD;
	UPROPERTY()
	class AShootingGalleryHUD* HUD;
	UPROPERTY()
	TArray<ATarget*> SpawnedTargets;
	UPROPERTY()
	int Score;
	UPROPERTY()
	int MissedShots;

	FTimerHandle StartGameTimerHandle;
	FTimerHandle GameTimerHandle;
	bool bIsGameActive;

public:
	UFUNCTION(BlueprintCallable)
	int GetRemainingMissedShots();
	UFUNCTION(BlueprintCallable)
	FString GetPrettyCurrentScore();
	UFUNCTION(BlueprintCallable)
	bool GetIsGameActive();
	UFUNCTION(BlueprintCallable)
	int GetTimeLeft();

private:
	void SpawnTargets();
	void StartGame();
	UFUNCTION()
	void OnTargetWasHit(ATarget* Target);
	void OnShotWasMissed();
	void AddToScore(int points);
	void ResetTarget(ATarget* Target);
	void RaiseTarget(ATarget* Target);
	void GameOver();
};
