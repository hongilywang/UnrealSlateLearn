// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiResourceObject.h"
#include "Components/StaticMeshComponent.h"
#include "SlAiTypes.h"
#include "SlAiDataHandle.h"
#include "SlAiHelper.h"
#include "SlAiFlobObject.h"

// Sets default values
ASlAiResourceObject::ASlAiResourceObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ʵ�������ڵ�
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	//ʵ����ģ�����
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("ResourceProfile"));

	//�����������
	BaseMesh->SetGenerateOverlapEvents(true);

	HP = BaseHP = 0;
}

// Called when the game starts or when spawned
void ASlAiResourceObject::BeginPlay()
{
	Super::BeginPlay();
	
	SlAiHelper::Debug(FString("ResourceObject Begin Play"), 10.f);

}

void ASlAiResourceObject::CreateFlobObject()
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	//�������ɵ�����
	for (TArray<TArray<int>>::TIterator It(ResourceAttr->FlobObjectInfo); It; ++It)
	{
		//������ɵ�����
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		//��������
		int Num = Stream.RandRange((*It)[1], (*It)[2]);

		if (GetWorld())
		{
			for (int i = 0; i < Num; ++i) 
			{
				//���ɵ�����
				ASlAiFlobObject* FlobObject = GetWorld()->SpawnActor<ASlAiFlobObject>(GetActorLocation() + FVector(0.f, 0.f, 20.f), FRotator::ZeroRotator);
				FlobObject->CreateFlobObject((*It)[0]);
			}
		}
	}
}

FText ASlAiResourceObject::GetInfoText()
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	if (HP <= 0)
	{
		HP = BaseHP = ResourceAttr->HP;
	}

	switch (SlAiDataHandle::Get()->CurrentCulture)
	{
	case ECultureTeam::EN:
		return ResourceAttr->EN;
	case ECultureTeam::ZH:
		return ResourceAttr->ZH;
	}
	return ResourceAttr->ZH;
}

float ASlAiResourceObject::GetHPRange()
{
	return FMath::Clamp<float>((float)HP / (float)BaseHP, 0.f, 1.f);
}

ASlAiResourceObject* ASlAiResourceObject::TakeObjectDamage(int Damge)
{
	HP = FMath::Clamp<int>(HP - Damge, 0, BaseHP);

	if (HP <= 0)
	{
		//���ʧЧ
		BaseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		//����������
		CreateFlobObject();
		//��������
		GetWorld()->DestroyActor(this);
	}
	return this;
}

// Called every frame
void ASlAiResourceObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EResourceType::Type ASlAiResourceObject::GetResourceType()
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	return ResourceAttr->ResourceType;
}

