// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPickupObject.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASlAiPickupObject::ASlAiPickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ʵ�������ڵ�
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	//ʵ����ģ�����
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("PickupProfile"));
}

// Called when the game starts or when spawned
void ASlAiPickupObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlAiPickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

