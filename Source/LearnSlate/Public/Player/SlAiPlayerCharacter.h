// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlAiPlayerCharacter.generated.h"

UCLASS()
class LEARNSLATE_API ASlAiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlAiPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class UCameraComponent* FirstCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		UCameraComponent* ThirdCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

private:
	//第一人称的骨骼模型
	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		USkeletalMeshComponent* MeshFirst;
};
