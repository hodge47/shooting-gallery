// Fill out your copyright notice in the Description page of Project Settings.


#include "HighScoreSaveGame.h"
#include "ShootingGallery.h"

UHighScoreSaveGame::UHighScoreSaveGame(){
	SaveSlotName = TEXT("high_score");
	UserIndex = 0;
}

// Adds leading zeros to high score - Modified function from https://mylittledevblog.wordpress.com/2018/02/15/ue4-add-leading-zeroes/
FString UHighScoreSaveGame::GetPrettyHighScore()
{
	char Buffer[100];
	snprintf(Buffer, 100, "%0*d", 7, HighScore);
	return FString(ANSI_TO_TCHAR(Buffer));
}
