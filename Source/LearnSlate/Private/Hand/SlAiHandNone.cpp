// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiHandNone.h"
#include "Components/BoxComponent.h"

ASlAiHandNone::ASlAiHandNone()
{
	//不给模型组件添加模型

	//设置碰撞体属性
	AffectCollision->SetRelativeLocation(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeScale3D(FVector(0.f, 0.f, 10.f));
}

void ASlAiHandNone::BeginPlay()
{
	Super::BeginPlay();

	//定义物品序号
	ObjectIndex = 0;
}
