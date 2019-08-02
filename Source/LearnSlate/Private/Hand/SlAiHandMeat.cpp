// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiHandMeat.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"

ASlAiHandMeat::ASlAiHandMeat()
{
	//给模型组件添加模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_Meat_02.SM_Prop_Meat_02'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(6.f, -7.044f, 2.62f));
	BaseMesh->SetRelativeRotation(FRotator(-50.f, 90.f, 0.f));
	BaseMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	//设置碰撞体属性
	AffectCollision->SetRelativeLocation(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeScale3D(FVector(0.f, 0.f, 10.f));
}

void ASlAiHandMeat::BeginPlay()
{
	Super::BeginPlay();

	//定义物品序号
	ObjectIndex = 4;
}
