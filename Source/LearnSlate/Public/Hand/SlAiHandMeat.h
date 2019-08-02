// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hand/SlAiHandObject.h"
#include "SlAiHandMeat.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API ASlAiHandMeat : public ASlAiHandObject
{
	GENERATED_BODY()

public:
	ASlAiHandMeat();

protected:

	virtual void BeginPlay() override;
};
