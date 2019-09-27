// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiPlayerCharacter.h"

ASlAiEnemyController::ASlAiEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlAiEnemyController::BeginPlay()
{
	//调用父类函数
	Super::BeginPlay();

	//
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ASlAiEnemyController::Tick(float DeltaTime)
{
}

FVector ASlAiEnemyController::GetPlayerLocation() const
{
	return FVector();
}


