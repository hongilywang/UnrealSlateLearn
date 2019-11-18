// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SlAiSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API USlAiSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		float PlayerHP;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		float PlayerHunger;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<int32> InputIndex;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<int32> InputNum;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<int32> NormalIndex;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<int32> NormalNum;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<int32> ShortcutIndex;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<int32> ShortcutNum;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<FVector> EnemyLocation;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<float> EnemyHP;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<FVector> ResourceRock;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<FVector> ResourceTree;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<FVector> PickupStone;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		TArray<FVector> PickupWood;
};
