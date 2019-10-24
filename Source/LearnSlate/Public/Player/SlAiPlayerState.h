// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SlAiTypes.h"
#include "SlAiPlayerState.generated.h"

class STextBlock;

//�������״̬UIί��
DECLARE_DELEGATE_TwoParams(FUpdateStateWidget, float, float)

/**
 * 
 */
UCLASS()
class LEARNSLATE_API ASlAiPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASlAiPlayerState();

	virtual void Tick(float DeltaSeconds) override;

	//�ṩ��ShortcutWidget����ӿ��������ί��
	void RegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* ContainerList, TSharedPtr<STextBlock> ShortcutInfoTextBlock);

	//�л������
	void ChooseShortcut(bool IsPre);

	//��ȡѡ����������Ʒ��index
	int GetCurrentHandObjectIndex() const;

	//��ȡ��ǰ�ֳ���Ʒ����Ʒ����
	EObjectType::Type GetCurrentObjectType();

	//�ṩ��RayInfowidget��ע��������Ϣ�¼�
	void RegisterRayInfoEvent(TSharedPtr<STextBlock> RayInfoTextBlock);

	//��ȡ������Ʒ�Ĺ�����Χ
	int GetAffectRange();

	//��ȡ�˺�
	int GetDamageValue(EResourceType::Type ResouceType);

	//���Ŀ������Ʒ��Ϣ
	void ChangeHandObject(int ShortcutID, int ObjectID, int ObjectNum);

	//��ʾ������
	void PromoteHunger();

	//�Ƿ��Ѿ�����
	bool IsPlayerDead();

	//�����˺�
	void AcceptDamage(int DamageValue);

protected:
	virtual void BeginPlay() override;

public:
	//��ǰ��ѡ�еĿ�������
	int CurrentShortcutIndex;

	//���߼����Ϣ
	FText RayInfoText;

	//�������״̬UI���󶨵ķ�����PlayerStateWidget��UpdateStateWidget
	FUpdateStateWidget UpdateStateWidget;

	//��ȡ������ָ��
	class ASlAiPlayerController* SPController;

private:
	//��ȡ�������Ʒ��Ϣ
	FText GetShortcutInfoText() const;

	//��ȡ���߼����Ϣ
	FText GetRayInfoText() const;

private:
	//�����
	TArray<TSharedPtr<ShortcutContainer>> ShortcutContainerList;

	//�������Ϣ����
	TAttribute<FText> ShortcutInfoTextAttr;

	//������Ϣ����
	TAttribute<FText> RayInfoTextAttr;

	//Ѫֵ ������
	float HP;
	float Hunger;

	//�Ƿ��Ѿ�����
	bool IsDead;
};
