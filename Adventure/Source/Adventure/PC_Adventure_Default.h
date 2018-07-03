// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Adventure_Default.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterStat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	int Value;

	UPROPERTY(BlueprintReadWrite)
	int MaxValue;
};

UCLASS()
class ADVENTURE_API APC_Adventure_Default : public APlayerController
{
	GENERATED_BODY()
	
	
	APC_Adventure_Default();
	
};