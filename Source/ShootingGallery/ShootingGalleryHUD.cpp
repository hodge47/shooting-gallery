// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGalleryHUD.h"

AShootingGalleryHUD::AShootingGalleryHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AShootingGalleryHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AShootingGalleryHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AShootingGalleryHUD::SetTargetController(ATargetController* AssignedTargetController)
{
	TargetController = AssignedTargetController;
}


void AShootingGalleryHUD::DrawHUD()
{
	Super::DrawHUD();

	if(GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		UIScale = ViewportSize.X * 2048.f;
	}

	if(TargetController)
	{
		DrawRemainingMissedShots();
	}
}

void AShootingGalleryHUD::DrawRemainingMissedShots()
{
	if(!TargetController) return;
	
	const FString RemainingMissedShots ("%f", TargetController->GetRemainingMissedShots());
	float TextWidth = 0.f;
	float TextHeight = 0.f;
	GetTextSize(RemainingMissedShots, TextWidth, TextHeight);
	DrawText(RemainingMissedShots, FLinearColor::White, 20.f, 20.f);
}




