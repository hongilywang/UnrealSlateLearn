// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyCharacter.h"

#include "SlAiEnemyController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASlAiEnemyCharacter::ASlAiEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����Ai������
	AIControllerClass = ASlAiEnemyController::StaticClass();

	//������ײ�������ļ�
	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyProfile"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASlAiEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlAiEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlAiEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

