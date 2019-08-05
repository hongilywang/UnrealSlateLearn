// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiHandObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"

#include "SlAiHandNone.h"
#include "SlAiHandSword.h"
#include "SlAiHandWood.h"
#include "SlAiHandStone.h"
#include "SlAiHandApple.h"
#include "SlAiHandMeat.h"
#include "SlAiHandAxe.h"
#include "SlAiHandHammer.h"

// Sets default values
ASlAiHandObject::ASlAiHandObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ʵ���������
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	//������̬ģ�����
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("NoCollision"));

	//ʵ������ײ���
	AffectCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AffectCollision"));
	AffectCollision->SetupAttachment(RootComponent);
	AffectCollision->SetCollisionProfileName(FName("ToolProfile"));

	//��ʼʱ�ر�Overlay���
	AffectCollision->SetGenerateOverlapEvents(false);

	//�󶨼�ⷽ������ײ��
	FScriptDelegate OverlayBegin;
	OverlayBegin.BindUFunction(this, "OnOverlayBegin");
	AffectCollision->OnComponentBeginOverlap.Add(OverlayBegin);

	FScriptDelegate OverlayEnd;
	OverlayEnd.BindUFunction(this, "OnOverlayEnd");
	AffectCollision->OnComponentEndOverlap.Add(OverlayEnd);
}

// Called when the game starts or when spawned
void ASlAiHandObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlAiHandObject::OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ASlAiHandObject::OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

// Called every frame
void ASlAiHandObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TSubclassOf<AActor> ASlAiHandObject::SpawnHandObject(int ObjectID)
{
	switch (ObjectID)
	{
	case 0:
		return ASlAiHandNone::StaticClass();
	case 1:
		return ASlAiHandWood::StaticClass();
	case 2:
		return ASlAiHandStone::StaticClass();
	case 3:
		return ASlAiHandApple::StaticClass();
	case 4:
		return ASlAiHandMeat::StaticClass();
	case 5:
		return ASlAiHandAxe::StaticClass();
	case 6:
		return ASlAiHandHammer::StaticClass();
	case 7:
		return ASlAiHandSword::StaticClass();
	}
	return ASlAiHandNone::StaticClass();
}

