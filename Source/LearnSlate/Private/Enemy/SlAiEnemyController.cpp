// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiEnemyCharacter.h"
#include "Camera/CameraComponent.h"

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
	if (!SECharacter)
		SECharacter = Cast<ASlAiEnemyCharacter>(GetPawn());
}

void ASlAiEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//每帧更新血条的位置朝向
	if (SECharacter && SPCharacter && SPCharacter->GetCurrentCamera())
		SECharacter->UpdateHPBarRotation(SPCharacter->GetCurrentCamera()->GetComponentLocation());
}

FVector ASlAiEnemyController::GetPlayerLocation() const
{
	//如果玩家指针存在，返回玩家位置
	if (SPCharacter)
		return SPCharacter->GetActorLocation();

	return FVector::ZeroVector;
}


