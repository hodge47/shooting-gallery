// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetController.h"
#include "GameFramework/HUD.h"
#include "ShootingGalleryHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGALLERY_API AShootingGalleryHUD : public AHUD
{
	GENERATED_BODY()

public:
	AShootingGalleryHUD();

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	void SetTargetController(ATargetController* AssignedTargetController);

private:
	virtual void DrawHUD() override;
	void DrawRemainingMissedShots();
	
private:
	class ATargetController* TargetController;
	float UIScale;
	
};
