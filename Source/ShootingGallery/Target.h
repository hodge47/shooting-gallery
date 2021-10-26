// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class SHOOTINGGALLERY_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// The target's animation instance class
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcces = "true"))
	class UTargetAnimInstance* TargetAnimInstance;

private:
	// The target mesh that will be shot at by the player
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* TargetMesh;
	// The amount of points the target is worth
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=Scoring, meta = (AllowPrivateAccess = "true"))
	int Points;
	// Is the target active? True = target is up in animation state, False = target is down in animation state
	bool bIsActive;
	

public:
	// Called by the target controller to tell this target to go inactive and play the falling animation
	void HitTarget();
	// Called by the target controller to tell this target to go active and play the raising animation
	void RaiseTarget();
	// Returns the points value of this target
	int GetPointsValue();
	// Set the target active or inactive based on the supplied argument
	void SetTargetActive(bool bTargetIsActive);
	// Returns the active state of the target
	bool GetIsActive();
	
};
