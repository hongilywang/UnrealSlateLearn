// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlAiGameMode.generated.h"

//��ʼ������������ί��
DECLARE_DELEGATE(FInitPackageManager)

/**
 * 
 */
UCLASS()
class LEARNSLATE_API ASlAiGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASlAiGameMode();

	//��д
	virtual void Tick(float DeltaSeconds) override;

	//�����ֵ�� ��GameHUD���ã�����������������
	void InitGamePlayModule();

public:
	class ASlAiPlayerController* SPController;
	class ASlAiPlayerCharacter* SPCharacter;
	class ASlAiPlayerState* SPState;
	//��ʼ����������ί�У��󶨵ķ�ʽPackageWidget��InitPackageManager����
	FInitPackageManager InitPackageManager;

protected:
	virtual void BeginPlay() override;

	void InitializePackage();

private:
	//�Ƿ��Ѿ���ʼ������
	bool IsInitPackage;
};
