// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Target.h"
#include "GameFramework/Pawn.h"
#include "ShooterController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetHit, ATarget*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShotMissed);

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
	
	UPROPERTY(BlueprintAssignable)
	FOnTargetHit OnTargetHit;
	UPROPERTY(BlueprintAssignable)
	FOnShotMissed OnShotMissed;

private:
	class USceneComponent* SceneRootComponent;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* GunMesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

private:
	void LookUp(float value);
	void LookRight(float value);
    void Fire();
};
