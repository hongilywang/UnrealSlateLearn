// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPickupStone.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

ASlAiPickupStone::ASlAiPickupStone()
{
	//��ģ����������ģ��
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_StoneBlock_01.SM_Prop_StoneBlock_01'"));
	//��ģ�͵�Mesh���
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.5f));

	ObjectIndex = 2;
}
