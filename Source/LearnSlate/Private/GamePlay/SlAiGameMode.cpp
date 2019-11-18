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
#include "SlAiSaveGame.h"
#include "SlAiResourceRock.h"
#include "SlAiResourceTree.h"
#include "SlAiPickupStone.h"
#include "SlAiPickupWood.h"

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
	//С��ͼ��û������
	IsCreateMiniMap = false;
	//��ʼ���㲻��Ҫ���ش浵
	IsNeedLoadRecord = false;
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	InitializeMiniMapCamera();

	//���������ش浵�����ڳ�ʼ������������Ϊ�˵ڶ�֡��ִ��
	LoadRecordPackage();

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

	LoadRecord();
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

void ASlAiGameMode::LoadRecord()
{
	//���RecordNameΪ�գ�ֱ��return
	if (SlAiDataHandle::Get()->RecordName.IsEmpty() || SlAiDataHandle::Get()->RecordName.Equals(FString("Default")))
		return;
	//ѭ�����浵�Ƿ��Ѿ�����
	for (TArray<FString>::TIterator It(SlAiDataHandle::Get()->RecordDataList); It; ++It)
	{
		if ((*It).Equals(SlAiDataHandle::Get()->RecordName))
		{
			IsNeedLoadRecord = true;
			break;
		}
	}

	//�����Ҫ���ش浵�����д浵����
	if (IsNeedLoadRecord && UGameplayStatics::DoesSaveGameExist(SlAiDataHandle::Get()->RecordName, 0))
		GameRecord = Cast<USlAiSaveGame>(UGameplayStatics::LoadGameFromSlot(SlAiDataHandle::Get()->RecordName, 0));
	else
		IsNeedLoadRecord = false;
	
	//�����Ҫ���ش浵���Ҵ浵����
	if (IsNeedLoadRecord && GameRecord)
	{
		//�������λ�ú�Ѫ��
		SPCharacter->SetActorLocation(GameRecord->PlayerLocation);
		SPState->LoadState(GameRecord->PlayerHP, GameRecord->PlayerHunger);

		//ѭ�����õ���
		int EnemyCount = 0;
		for (TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt)
		{
			if (EnemyCount < GameRecord->EnemyLocation.Num())
			{
				(*EnemyIt)->SetActorLocation(GameRecord->EnemyLocation[EnemyCount]);
				(*EnemyIt)->LoadHP(GameRecord->EnemyHP[EnemyCount]);
			}
			else
				(*EnemyIt)->IsDestroyNextTick = true;
			++EnemyCount;
		}

		//ѭ��������ʯ
		int RockCount = 0;
		for (TActorIterator<ASlAiResourceRock> RockIt(GetWorld()); RockIt; ++RockIt)
		{
			if (RockCount < GameRecord->ResourceRock.Num())
			{
				(*RockIt)->SetActorLocation(GameRecord->ResourceRock[RockCount]);
			}
			else
			{
				(*RockIt)->IsDestroyNextTick = true;
			}
			++RockCount;
		}

		int TreeCount = 0;
		for (TActorIterator<ASlAiResourceTree> TreeIt(GetWorld()); TreeIt; ++TreeIt)
		{
			if (TreeCount < GameRecord->ResourceTree.Num())
			{
				(*TreeIt)->SetActorLocation(GameRecord->ResourceTree[TreeCount]);
			}
			else
			{
				(*TreeIt)->IsDestroyNextTick = true;
			}
			++TreeCount;
		}

		int StoneCount = 0;
		for (TActorIterator<ASlAiPickupStone> StoneIt(GetWorld()); StoneIt; ++StoneIt)
		{
			if (StoneCount < GameRecord->PickupStone.Num())
			{
				(*StoneIt)->SetActorLocation(GameRecord->PickupStone[StoneCount]);
			}
			else
			{
				(*StoneIt)->IsDestroyNextTick = true;
			}
			++StoneCount;
		}

		int WoodCount = 0;
		for (TActorIterator<ASlAiPickupWood> WoodIt(GetWorld()); WoodIt; ++WoodIt)
		{
			if (WoodCount < GameRecord->PickupWood.Num())
			{
				(*WoodIt)->SetActorLocation(GameRecord->PickupWood[WoodCount]);
			}
			else
			{
				(*WoodIt)->IsDestroyNextTick = true;
			}
			++WoodCount;
		}
	}
}

void ASlAiGameMode::LoadRecordPackage()
{
	if (!IsInitPackage || !IsNeedLoadRecord)
		return;

	if (IsNeedLoadRecord && GameRecord)
	{
		SlAiPackageManager::Get()->LoadRecord(&GameRecord->InputIndex, &GameRecord->InputNum, &GameRecord->NormalIndex, &GameRecord->NormalNum, &GameRecord->ShortcutIndex, &GameRecord->ShortcutNum);
	}

	IsNeedLoadRecord = false;
}
