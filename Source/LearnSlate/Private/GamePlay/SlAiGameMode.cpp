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
#include "SlAiSceneCapture2D.h"
#include "SlAiEnemyCharacter.h"
#include "EngineUtils.h"
#include "Sound/SoundWave.h"

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
	InitializeMiniMapCamera();

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

	//���ű�������
	USoundWave* BGMusic = LoadObject<USoundWave>(nullptr, TEXT("SoundWave'/Game/Res/Sound/GameSound/GameBG.GameBG'"));
	BGMusic->bLooping = true;
	UGameplayStatics::PlaySound2D(GetWorld(), BGMusic, 0.1f);
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

void ASlAiGameMode::InitializeMiniMapCamera()
{
	if (!IsCreateMiniMap && GetWorld())
	{
		//�ݳ�С��ͼ�����
		MiniMapCamera = GetWorld()->SpawnActor<ASlAiSceneCapture2D>(ASlAiSceneCapture2D::StaticClass());
		RegisterMiniMap.ExecuteIfBound(MiniMapCamera->GetMiniMapTex());
		//���޸�С��ͼ��Ұί��
		SPController->UpdateMiniMapWidth.BindUObject(MiniMapCamera, &ASlAiSceneCapture2D::UpdateMiniMapWidth);
		IsCreateMiniMap = true;
	}

	if (IsCreateMiniMap)
	{
		MiniMapCamera->UpdateTransform(SPCharacter->GetActorLocation(), SPCharacter->GetActorRotation());

		TArray<FVector2D> EnemyPosList;
		TArray<bool> EnemyLockList;
		TArray<float> EnemyRotateList;

		//��ȡ�����еĵ���
		for (TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt)
		{
			FVector EnemyPos = FVector((*EnemyIt)->GetActorLocation().X - SPCharacter->GetActorLocation().X, (*EnemyIt)->GetActorLocation().Y - SPCharacter->GetActorLocation().Y, 0.f);

			EnemyPos = FQuat(FVector::UpVector, FMath::DegreesToRadians(-SPCharacter->GetActorRotation().Yaw - 90.f)) * EnemyPos;

			EnemyPosList.Add(FVector2D(EnemyPos.X, EnemyPos.Y));
			EnemyLockList.Add((*EnemyIt)->IsLockPlayer());
			EnemyRotateList.Add((*EnemyIt)->GetActorRotation().Yaw - SPCharacter->GetActorRotation().Yaw);
		}

		//ÿ֡����С��ͼ�ķ�������λ��
		UpdateMapData.ExecuteIfBound(SPCharacter->GetActorRotation(), MiniMapCamera->GetMapSize(), &EnemyPosList, &EnemyLockList, &EnemyRotateList);
	}
}
