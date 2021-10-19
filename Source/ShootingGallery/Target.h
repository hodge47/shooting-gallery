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
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcces = "true"))
	class UTargetAnimInstance* TargetAnimInstance;

private:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* TargetMesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=Scoring, meta = (AllowPrivateAccess = "true"))
	int Points;

	bool bIsActive;
	

public:
	void HitTarget();
	void RaiseTarget();
	int GetPointsValue();
	void SetTargetActive(bool bTargetIsActive);
	bool GetIsActive();
	
};
