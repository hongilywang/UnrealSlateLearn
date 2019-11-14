// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiHandAxe.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"

ASlAiHandAxe::ASlAiHandAxe()
{
	//��ģ��������ģ��
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Wep_Axe_01.SM_Wep_Axe_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(-28.f, -4.9f, 3.23f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, -90.f, 90.f));
	BaseMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	//������ײ������
	AffectCollision->SetRelativeLocation(FVector(32.f, -5.f, 3.f));
	AffectCollision->SetRelativeScale3D(FVector(0.375f, 0.5f, 0.125f));
}

void ASlAiHandAxe::BeginPlay()
{
	Super::BeginPlay();

	//������Ʒ���
	ObjectIndex = 5;

	OverlaySound = LoadObject<USoundWave>(nullptr, TEXT("SoundWave'/Game/Res/Sound/GameSound/Axe.Axe'"));
}
