// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SlAiTypes.h"
#include "SlAiPlayerController.generated.h"

//�޸�׼��ί��
DECLARE_DELEGATE_TwoParams(FUpdatePointer, bool, float)

/**
 * 
 */
UCLASS()
class LEARNSLATE_API ASlAiPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASlAiPlayerController();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	//��character���ֳ���Ʒ���и��ģ����������playerstate�ڻ����
	void ChangeHandObject();

public:
	//��ȡ��ҽ�ɫ
	class ASlAiPlayerCharacter* SPCharacter;
	class ASlAiPlayerState* SPState;

	//ʵʱ�޸�׼�ǵ�ί�У�ע��ĺ���Pointerwidget��UpdatePointer
	FUpdatePointer UpdatePointer;

protected:
	virtual void BeginPlay() override;

private:
	//�л��ӽ�
	void ChangeView();

	//��갴���¼�
	void LeftEventStart();
	void LeftEventStop();
	void RightEventStart();
	void RightEventStop();

	//�������¹����¼�
	void ScrollUpEvent();
	void ScrollDownEvent();

	//�޸�Ԥ����
	void ChangePreUpperType(EUpperBody::Type RightType);

private:
	//���Ԥ����
	EUpperBody::Type LeftUpperType;

	//�Ҽ�Ԥ����
	EUpperBody::Type RightUpperType;

	//�Ƿ�ס��������
	bool IsLeftButtonDown;
	bool IsRightButtonDown;
};
