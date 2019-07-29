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
	//������tick����
	PrimaryActorTick.bCanEverTick = true;

	//������
	HUDClass = ASlAiGameHUD::StaticClass();
	PlayerControllerClass = ASlAiPlayerController::StaticClass();
	PlayerStateClass = ASlAiPlayerState::StaticClass();
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{

}

void ASlAiGameMode::BeginPlay()
{
	//��ʼ����Ϸ����
	SlAiDataHandle::Get()->InitializeGameData();
}
