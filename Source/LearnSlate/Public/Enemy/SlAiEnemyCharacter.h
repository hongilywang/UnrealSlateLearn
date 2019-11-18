// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlAiTypes.h"
#include "SlAiEnemyCharacter.generated.h"

UCLASS()
class LEARNSLATE_API ASlAiEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlAiEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//���ɵ�����
	void CreateFlobObject();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ʵʱ����Ѫ���ĳ��� ��controller���ã��������λ��
	void UpdateHPBarRotation(FVector SPLocation);

	//�޸��ƶ��ٶ�
	void SetMaxSpeed(float Speed);

	//��ȡ�ȴ�ʱ��
	float GetIdleWaitTime();

	//���Ź������������ع���ʱ��
	float PlayAttackAction(EEnemyAttackType AttackType);

	//���ܹ�����������дAPawn��TaskDamage����
	void AcceptDamage(int DamageValue);

	//�������˶���
	float PlayHurtAction();

	//��������
	void StartDefence();

	//ֹͣ����
	void StopDefence();

	//���ٺ���
	void DestroyEvent();

	//��ȡ��Ʒ��Ϣ
	FText GetInfoText() const;

	//�޸��ֳ���Ʒ����ײ����Ƿ���
	void ChangeWeaponDetect(bool IsOpen);

	//��ȡ�Ƿ��Ѿ����������
	bool IsLockPlayer();

	//����Ѫ��
	void LoadHP(float HPValue);

public:
	//��ԴID
	int ResourceIndex;

	//�Ƿ�����һ֡�����Լ�����GameMode������Ϸ�浵ʱ��������
	bool IsDestroyNextTick;

protected:
	//�������
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* WeaponSocket;

	//���Ʋ��
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* SheildSocket;

	//Ѫ��
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UWidgetComponent* HPBar;

	//���˸�֪
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UPawnSensingComponent* EnemySense;

private:
	//�󶨵����˸�֪�ķ���
	UFUNCTION()
		void OnSeePlayer(APawn* PlayerChar);

	//Ѫ��UI����
	TSharedPtr<class SSlAiEnemyHPWidget> HPBarWidget;

	//����������
	class ASlAiEnemyController* SEController;

	//Ѫ��
	float HP;

	//��������
	class USlAiEnemyAnim* SEAnim;

	//����������Դ
	class UAnimationAsset* AnimDead_I;
	class UAnimationAsset* AnimDead_II;

	//����ʱ��ί��
	FTimerHandle DeadHandle;
};
