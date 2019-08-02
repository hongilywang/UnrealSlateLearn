// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiHandWood.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"

ASlAiHandWood::ASlAiHandWood()
{
	//给模型组件添加模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_TreeLog_01.SM_Env_TreeLog_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));
	BaseMesh->SetRelativeScale3D(FVector(0.1f,0.1f, 0.1f));

	//设置碰撞体属性
	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
}

void ASlAiHandWood::BeginPlay()
{
	Super::BeginPlay();

	//定义物品序号
	ObjectIndex = 1;
}
