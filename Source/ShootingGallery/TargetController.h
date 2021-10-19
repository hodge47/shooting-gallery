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
	FVector2D TargetGridSize;
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector2D TargetGridSpacing;
	UPROPERTY(EditDefaultsOnly, Category="Target Resetting")
	float ResetTimeSeconds;

private:
	UPROPERTY()
	class AShooterController* PlayerController;
	TArray<ATarget*> SpawnedTargets;
	int Score;

private:
	void SpawnTargets();
	UFUNCTION()
	void OnTargetWasHit(ATarget* Target);
	void AddToScore(int points);
	void ResetTarget(ATarget* Target);
	void RaiseTarget(ATarget* Target);

};
