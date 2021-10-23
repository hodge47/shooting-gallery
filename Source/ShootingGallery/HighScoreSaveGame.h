// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighScoreSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGALLERY_API UHighScoreSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UHighScoreSaveGame();
	
	UPROPERTY(VisibleAnywhere, Category="Scoring")
	int HighScore;

	UPROPERTY(VisibleAnywhere, Category="Scoring")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category="Scoring")
	uint32 UserIndex;
};
