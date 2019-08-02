// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiHandApple.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"

ASlAiHandApple::ASlAiHandApple()
{
	//给模型组件添加模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Item_Fruit_02.SM_Item_Fruit_02'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(-8.f, -3.f, 7.f));
	BaseMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	BaseMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	//设置碰撞体属性
	AffectCollision->SetRelativeLocation(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeScale3D(FVector(0.f, 0.f, 10.f));
}

void ASlAiHandApple::BeginPlay()
{
	Super::BeginPlay();

	//定义物品序号
	ObjectIndex = 3;
}
