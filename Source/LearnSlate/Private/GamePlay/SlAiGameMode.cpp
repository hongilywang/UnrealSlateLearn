// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameMode.h"
#include "SlAiPlayerController.h"
#include "SlAiPlayerState.h"
#include "SlAiGameHUD.h"
#include "SlAiPlayerCharacter.h"

#include "SlAiDataHandle.h"
#include "SlAiHelper.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiGameInstance.h"

ASlAiGameMode::ASlAiGameMode()
{
	//允许开启tick函数
	PrimaryActorTick.bCanEverTick = true;

	//添加组件
	HUDClass = ASlAiGameHUD::StaticClass();
	PlayerControllerClass = ASlAiPlayerController::StaticClass();
	PlayerStateClass = ASlAiPlayerState::StaticClass();
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();

	//是否已经初始化背包
	IsInitPackage = false;
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	//TODO这样做好吗，每帧斗湖运行
	InitializePackage();
}

void ASlAiGameMode::InitGamePlayModule()
{
	//添加引用
	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SPState = Cast<ASlAiPlayerState>(SPCharacter->GetPlayerState());
}

void ASlAiGameMode::BeginPlay()
{
	SlAiHelper::Debug(FString("ASlAiGameMode Begin Play"), 10.f);
	//初始化游戏数据
	SlAiDataHandle::Get()->InitializeGameData();

	if (!SPController)
		InitGamePlayModule();
}

void ASlAiGameMode::InitializePackage()
{
	if (IsInitPackage)
		return;
	InitPackageManager.ExecuteIfBound();
	IsInitPackage = true;
}
