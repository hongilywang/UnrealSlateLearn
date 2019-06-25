// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LEARNSLATE_API SlAiInternation
{
public:
	static void Register(FText value)
	{
		return;
	}
};

#define LOCTEXT_NAMESPACE "SlAiMenu"
SlAiInternation::Register(LOCTEXT("Menu", "Menu"));
#undef LOCTEXT_NAMESPACE