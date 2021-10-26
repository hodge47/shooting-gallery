// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "ShooterController.generated.h"

// Hit and shot missed delegate declarations
DECLARE_DELEGATE_OneParam(FOnTargetHit, class ATarget*);
DECLARE_DELEGATE(FOnShotMissed);

UCLASS()
class SHOOTINGGALLERY_API AShooterController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShooterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Hit and shot missed delegates
	FOnTargetHit OnTargetHit;
	FOnShotMissed OnShotMissed;

private:
	// Root scene component
	class USceneComponent* SceneRootComponent;
	// Spring arm component to operate camera
	UPROPERTY(EditDefaultsOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	// The main game camera
	UPROPERTY(EditDefaultsOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	// Shot hit sound cue
	UPROPERTY(EditDefaultsOnly, Category="Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* ShotHitSoundCue;
	// Shot missed sound cue
	UPROPERTY(EditDefaultsOnly, Category="Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* ShotMissedSoundCue;
	// Particles to spawn on line trace blocking hit
	UPROPERTY(EditDefaultsOnly, Category="Particles", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticles;
	
	// Target controller reference
	class ATargetController* TargetController;

private:
	// Input bound function to look up and down
	void LookUp(float value);
	// Input bound function to look right and left
	void LookRight(float value);
	// Input bound function to fire bb gun
    void Fire();
};
