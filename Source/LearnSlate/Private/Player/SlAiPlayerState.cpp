// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPlayerState.h"
#include "STextBlock.h"
#include "SlAiDataHandle.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiPlayerController.h"

ASlAiPlayerState::ASlAiPlayerState()
{
	//运行运行tick函数
	PrimaryActorTick.bCanEverTick = true;

	//当前选中的快捷栏序号
	CurrentShortcutIndex = 0;

	//初始化
	HP = 500.f;
	Hunger = 600.f;
}

void ASlAiPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//饥饿度为0， 持续扣血，每秒2点
	if (Hunger <= 0)
	{
		HP -= DeltaSeconds * 2;
	}
	else
	{
		//如果饥饿度大于0，持续扣饥饿度，每秒减2
		//血量持续增加，每秒为1
		Hunger -= DeltaSeconds * 2;
		HP += DeltaSeconds;
	}
	HP = FMath::Clamp<float>(HP, 0.f, 500.f);
	Hunger = FMath::Clamp<float>(Hunger, 0.f, 600.f);
	//执行修改状态UI委托
	UpdateStateWidget.ExecuteIfBound(HP / 500.f, Hunger / 500.f);
}

void ASlAiPlayerState::RegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* ContainerList, TSharedPtr<STextBlock> ShortcutInfoTextBlock)
{
	for (TArray<TSharedPtr<ShortcutContainer>>::TIterator It(*ContainerList); It; ++It)
	{
		ShortcutContainerList.Add(*It);
	}
	ShortcutInfoTextAttr.BindUObject(this, &ASlAiPlayerState::GetShortcutInfoText);

	//绑定快捷栏信息TextBlock
	ShortcutInfoTextBlock->SetText(ShortcutInfoTextAttr);

	//临时代码
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
	//下一个被选中的容器的下标
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

//根据正在敲的资源的类型以及手持物品的类型来返回伤害
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
	//更改快捷栏信息
	ShortcutContainerList[ShortcutID]->SetObject(ObjectID)->SetObjectNum(ObjectNum);
	//告诉SPController更新一次手持物品
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
