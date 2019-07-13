// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlAiGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API ASlAiGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASlAiGameMode();

	//ÖØÐ´
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
};
