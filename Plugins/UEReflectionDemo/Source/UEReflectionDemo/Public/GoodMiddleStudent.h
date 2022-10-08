// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiddleStudent.h"
#include "GoodMiddleStudent.generated.h"

/**
 * 
 */
UCLASS()
class UEREFLECTIONDEMO_API UGoodMiddleStudent : public UMiddleStudent
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void Play(FString InGame);

	UFUNCTION()
		int GoHome();
};
