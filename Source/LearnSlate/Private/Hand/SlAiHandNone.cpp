// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiHandNone.h"
#include "Components/BoxComponent.h"

ASlAiHandNone::ASlAiHandNone()
{
	//����ģ��������ģ��

	//������ײ������
	AffectCollision->SetRelativeLocation(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeScale3D(FVector(0.f, 0.f, 10.f));
}

void ASlAiHandNone::BeginPlay()
{
	Super::BeginPlay();

	//������Ʒ���
	ObjectIndex = 0;
}
