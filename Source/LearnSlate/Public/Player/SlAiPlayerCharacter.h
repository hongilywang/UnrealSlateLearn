// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlAiTypes.h"
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

	//�޸��ӽ�
	void ChangeView(EGameViewMode::Type NewGameView);

	//�޸ĵ�ǰ���ֳ���Ʒ
	void ChangeHandObject(TSubclassOf<class AActor> HandObjectClass);

	//�޸��ֳ���Ʒ����ײ����Ƿ���
	void ChangeHandObjectDetect(bool IsOpen);

	//�Ƿ���Ⱦ������Ʒ����Anim����
	void RenderHandObject(bool IsRender);

public:

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class UCameraComponent* FirstCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class UCameraComponent* ThirdCamera;

	//��ǰ���ӽ�ģʽ
	EGameViewMode::Type GameView;

	//�ϰ�����״̬
	EUpperBody::Type UpperType;

	//�Ƿ������л��ӽ�
	bool IsAllowSwitch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//�ƶ���صķ���
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUpAtRate(float Value);
	void Turn(float Value);
	void TurnAtRate(float Value);
	void OnStartJump();
	void OnStopJump();
	void OnStartRun();
	void OnStopRun();

private:
	//��һ�˳ƵĹ���ģ��
	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		USkeletalMeshComponent* MeshFirst;

	//������Ʒ
	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class UChildActorComponent* HandObject;

	//��ת����
	float BaseLookUpRate;
	float BaseTurnRate;
};
