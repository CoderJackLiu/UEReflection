// Fill out your copyright notice in the Description page of Project Settings.


#include "GoodMiddleStudent.h"

void UGoodMiddleStudent::Play(FString InGame)
{
	UE_LOG(LogTemp, Warning, TEXT("Play function is called by name with param : %s."), *InGame);
}

int UGoodMiddleStudent::GoHome()
{
	UE_LOG(LogTemp, Warning, TEXT("GoHome function is called."));
	return 2;
}
