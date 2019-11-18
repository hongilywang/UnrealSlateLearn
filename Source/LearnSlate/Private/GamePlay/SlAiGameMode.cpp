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
	//允许开启tick函数
	PrimaryActorTick.bCanEverTick = true;

	//添加组件
	HUDClass = ASlAiGameHUD::StaticClass();
	PlayerControllerClass = ASlAiPlayerController::StaticClass();
	PlayerStateClass = ASlAiPlayerState::StaticClass();
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();

	//是否已经初始化背包
	IsInitPackage = false;
	//小地图还没有生成
	IsCreateMiniMap = false;
	//开始涉足不需要加载存档
	IsNeedLoadRecord = false;
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	InitializeMiniMapCamera();

	//给背包加载存档，放在初始化背包上面是为了第二帧再执行
	LoadRecordPackage();

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
	//初始化游戏数据
	SlAiDataHandle::Get()->InitializeGameData();

	if (!SPController)
		InitGamePlayModule();

	//播放背景音乐
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
	//绑定丢弃物品委托
	SlAiPackageManager::Get()->PlayerThrowObject.BindUObject(SPCharacter, &ASlAiPlayerCharacter::PlayerThrowObject);
	//绑定修改快捷栏信息
	SlAiPackageManager::Get()->ChangeHandObject.BindUObject(SPState, &ASlAiPlayerState::ChangeHandObject);

	IsInitPackage = true;
}

void ASlAiGameMode::InitializeMiniMapCamera()
{
	if (!IsCreateMiniMap && GetWorld())
	{
		//奢侈小地图摄像机
		MiniMapCamera = GetWorld()->SpawnActor<ASlAiSceneCapture2D>(ASlAiSceneCapture2D::StaticClass());
		RegisterMiniMap.ExecuteIfBound(MiniMapCamera->GetMiniMapTex());
		//绑定修改小地图视野委托
		SPController->UpdateMiniMapWidth.BindUObject(MiniMapCamera, &ASlAiSceneCapture2D::UpdateMiniMapWidth);
		IsCreateMiniMap = true;
	}

	if (IsCreateMiniMap)
	{
		MiniMapCamera->UpdateTransform(SPCharacter->GetActorLocation(), SPCharacter->GetActorRotation());

		TArray<FVector2D> EnemyPosList;
		TArray<bool> EnemyLockList;
		TArray<float> EnemyRotateList;

		//获取场景中的敌人
		for (TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt)
		{
			FVector EnemyPos = FVector((*EnemyIt)->GetActorLocation().X - SPCharacter->GetActorLocation().X, (*EnemyIt)->GetActorLocation().Y - SPCharacter->GetActorLocation().Y, 0.f);

			EnemyPos = FQuat(FVector::UpVector, FMath::DegreesToRadians(-SPCharacter->GetActorRotation().Yaw - 90.f)) * EnemyPos;

			EnemyPosList.Add(FVector2D(EnemyPos.X, EnemyPos.Y));
			EnemyLockList.Add((*EnemyIt)->IsLockPlayer());
			EnemyRotateList.Add((*EnemyIt)->GetActorRotation().Yaw - SPCharacter->GetActorRotation().Yaw);
		}

		//每帧更新小地图的方向文字位置
		UpdateMapData.ExecuteIfBound(SPCharacter->GetActorRotation(), MiniMapCamera->GetMapSize(), &EnemyPosList, &EnemyLockList, &EnemyRotateList);
	}
}

void ASlAiGameMode::LoadRecord()
{
	//如果RecordName为空，直接return
	if (SlAiDataHandle::Get()->RecordName.IsEmpty() || SlAiDataHandle::Get()->RecordName.Equals(FString("Default")))
		return;
	//循环检测存档是否已经存在
	for (TArray<FString>::TIterator It(SlAiDataHandle::Get()->RecordDataList); It; ++It)
	{
		if ((*It).Equals(SlAiDataHandle::Get()->RecordName))
		{
			IsNeedLoadRecord = true;
			break;
		}
	}

	//如果需要加载存档，进行存档加载
	if (IsNeedLoadRecord && UGameplayStatics::DoesSaveGameExist(SlAiDataHandle::Get()->RecordName, 0))
		GameRecord = Cast<USlAiSaveGame>(UGameplayStatics::LoadGameFromSlot(SlAiDataHandle::Get()->RecordName, 0));
	else
		IsNeedLoadRecord = false;
	
	//如果需要加载存档并且存档存在
	if (IsNeedLoadRecord && GameRecord)
	{
		//涉足玩家位置和血量
		SPCharacter->SetActorLocation(GameRecord->PlayerLocation);
		SPState->LoadState(GameRecord->PlayerHP, GameRecord->PlayerHunger);

		//循环设置敌人
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

		//循环设置岩石
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
