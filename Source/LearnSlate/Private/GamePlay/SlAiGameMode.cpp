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
#include "SlAiPackageManager.h"

ASlAiGameMode::ASlAiGameMode()
{
	//������tick����
	PrimaryActorTick.bCanEverTick = true;

	//������
	HUDClass = ASlAiGameHUD::StaticClass();
	PlayerControllerClass = ASlAiPlayerController::StaticClass();
	PlayerStateClass = ASlAiPlayerState::StaticClass();
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();

	//�Ƿ��Ѿ���ʼ������
	IsInitPackage = false;
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	//TODO����������ÿ֡��������
	InitializePackage();
}

void ASlAiGameMode::InitGamePlayModule()
{
	//�������
	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SPState = Cast<ASlAiPlayerState>(SPCharacter->GetPlayerState());
}

void ASlAiGameMode::BeginPlay()
{
	//��ʼ����Ϸ����
	SlAiDataHandle::Get()->InitializeGameData();

	if (!SPController)
		InitGamePlayModule();
}

void ASlAiGameMode::InitializePackage()
{
	if (IsInitPackage)
		return;
	InitPackageManager.ExecuteIfBound();
	//�󶨶�����Ʒί��
	SlAiPackageManager::Get()->PlayerThrowObject.BindUObject(SPCharacter, &ASlAiPlayerCharacter::PlayerThrowObject);
	//���޸Ŀ������Ϣ
	SlAiPackageManager::Get()->ChangeHandObject.BindUObject(SPState, &ASlAiPlayerState::ChangeHandObject);

	IsInitPackage = true;
}
