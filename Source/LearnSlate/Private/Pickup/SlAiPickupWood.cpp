// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPickupWood.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

ASlAiPickupWood::ASlAiPickupWood()
{
	//��ģ�����������ģ��
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_Loghalf_01.SM_Prop_Loghalf_01'"));
	//��ģ�͵�Mesh���
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeScale3D(FVector(0.4f));

	ObjectIndex = 1;
}