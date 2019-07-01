// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include <Engine.h>

namespace SlAiHelper
{
	FORCEINLINE void Debug(FString Mesaage, float Duration = 3.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Mesaage);
		}
	}
}
