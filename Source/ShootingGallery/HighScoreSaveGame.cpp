// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGallery.h"
#include "HighScoreSaveGame.h"

UHighScoreSaveGame::UHighScoreSaveGame(){
	SaveSlotName = TEXT("high_score");
	UserIndex = 0;
}