// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Student.generated.h"
/**
 * 
 */
UCLASS(MinimalAPI)
class UStudent : public UObject
{
	GENERATED_BODY()
public:
	UStudent();
	~UStudent();

	UPROPERTY(EditAnywhere,Category = "Category1")
	FString Name = "ZhangSan";

	UFUNCTION(BlueprintCallable)
	int Study(const FString& InWhat);

	UFUNCTION()
	const int Function1();

	UFUNCTION()
	void Function2(FString& InParam);
};
