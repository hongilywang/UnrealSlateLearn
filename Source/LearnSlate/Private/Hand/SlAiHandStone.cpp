// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiHandStone.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"

ASlAiHandStone::ASlAiHandStone()
{
	//��ģ��������ģ��
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_StoneBlock_01.SM_Prop_StoneBlock_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(1.f, -1.414f, 7.071f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, 0.f, -135.f));
	BaseMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	//������ײ������
	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
}

void ASlAiHandStone::BeginPlay()
{
	Super::BeginPlay();

	//������Ʒ���
	ObjectIndex = 2;
}
