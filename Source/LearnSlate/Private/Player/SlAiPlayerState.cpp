// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPlayerState.h"
#include "STextBlock.h"
#include "SlAiDataHandle.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiPlayerController.h"

ASlAiPlayerState::ASlAiPlayerState()
{
	//��������tick����
	PrimaryActorTick.bCanEverTick = true;

	//��ǰѡ�еĿ�������
	CurrentShortcutIndex = 0;

	//��ʼ��
	HP = 500.f;
	Hunger = 600.f;
}

void ASlAiPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//������Ϊ0�� ������Ѫ��ÿ��2��
	if (Hunger <= 0)
	{
		HP -= DeltaSeconds * 2;
	}
	else
	{
		//��������ȴ���0�������ۼ����ȣ�ÿ���2
		//Ѫ���������ӣ�ÿ��Ϊ1
		Hunger -= DeltaSeconds * 2;
		HP += DeltaSeconds;
	}
	HP = FMath::Clamp<float>(HP, 0.f, 500.f);
	Hunger = FMath::Clamp<float>(Hunger, 0.f, 600.f);
	//ִ���޸�״̬UIί��
	UpdateStateWidget.ExecuteIfBound(HP / 500.f, Hunger / 500.f);
}

void ASlAiPlayerState::RegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* ContainerList, TSharedPtr<STextBlock> ShortcutInfoTextBlock)
{
	for (TArray<TSharedPtr<ShortcutContainer>>::TIterator It(*ContainerList); It; ++It)
	{
		ShortcutContainerList.Add(*It);
	}
	ShortcutInfoTextAttr.BindUObject(this, &ASlAiPlayerState::GetShortcutInfoText);

	//�󶨿������ϢTextBlock
	ShortcutInfoTextBlock->SetText(ShortcutInfoTextAttr);

	//��ʱ����
	ShortcutContainerList[1]->SetObject(1)->SetObjectNum(5);
	ShortcutContainerList[2]->SetObject(2)->SetObjectNum(15);
	ShortcutContainerList[3]->SetObject(3)->SetObjectNum(25);
	ShortcutContainerList[4]->SetObject(4)->SetObjectNum(35);
	ShortcutContainerList[5]->SetObject(5)->SetObjectNum(45);
	ShortcutContainerList[6]->SetObject(6)->SetObjectNum(55);
	ShortcutContainerList[7]->SetObject(7)->SetObjectNum(65);
}

void ASlAiPlayerState::ChooseShortcut(bool IsPre)
{
	//��һ����ѡ�е��������±�
	int NextIndex = 0;
	if (IsPre)
		NextIndex = CurrentShortcutIndex - 1 < 0 ? 8 : CurrentShortcutIndex - 1;
	else
		NextIndex = CurrentShortcutIndex + 1 > 8 ? 0 : CurrentShortcutIndex + 1;
	ShortcutContainerList[CurrentShortcutIndex]->SetChoosed(false);
	ShortcutContainerList[NextIndex]->SetChoosed(true);
	CurrentShortcutIndex = NextIndex;
}

int ASlAiPlayerState::GetCurrentHandObjectIndex() const
{
	if (ShortcutContainerList.Num() == 0)
		return 0;
	
	return ShortcutContainerList[CurrentShortcutIndex]->ObjectIndex;

}

EObjectType::Type ASlAiPlayerState::GetCurrentObjectType()
{
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());
	return ObjectAttr->ObjectType;
}

void ASlAiPlayerState::RegisterRayInfoEvent(TSharedPtr<STextBlock> RayInfoTextBlock)
{
	RayInfoTextAttr.BindUObject(this, &ASlAiPlayerState::GetRayInfoText);
	RayInfoTextBlock->SetText(RayInfoTextAttr);
}

int ASlAiPlayerState::GetAffectRange()
{
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());

	return ObjectAttr->AffectRange;
}

//���������õ���Դ�������Լ��ֳ���Ʒ�������������˺�
int ASlAiPlayerState::GetDamageValue(EResourceType::Type ResouceType)
{
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());

	switch (ResouceType)
	{
	case EResourceType::Plant:
		return ObjectAttr->PlantAttack;
	case EResourceType::Metal:
		return ObjectAttr->MetalAttack;
	case EResourceType::Animal:
		return ObjectAttr->AnimalAttack;
	}
	return ObjectAttr->PlantAttack;
}

void ASlAiPlayerState::ChangeHandObject(int ShortcutID, int ObjectID, int ObjectNum)
{
	//���Ŀ������Ϣ
	ShortcutContainerList[ShortcutID]->SetObject(ObjectID)->SetObjectNum(ObjectNum);
	//����SPController����һ���ֳ���Ʒ
	SPController->ChangeHandObject();
}

void ASlAiPlayerState::BeginPlay()
{
	Super::BeginPlay();
	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

FText ASlAiPlayerState::GetShortcutInfoText() const
{
	TSharedPtr<ObjectAttribute> ObjectAttr;
	ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(GetCurrentHandObjectIndex());
	switch (SlAiDataHandle::Get()->CurrentCulture)
	{
	case ECultureTeam::EN:
		return ObjectAttr->EN;
	case ECultureTeam::ZH:
		return ObjectAttr->ZH;
	}
	return ObjectAttr->ZH;
}

FText ASlAiPlayerState::GetRayInfoText() const
{
	return RayInfoText;
}
