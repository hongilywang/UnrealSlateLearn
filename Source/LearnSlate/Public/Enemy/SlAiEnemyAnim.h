// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlAiTypes.h"
#include "SlAiEnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API USlAiEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USlAiEnemyAnim();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//����Idleģʽ�����ض���ʱ��
	float SetIdelType(int NewType);

	//���Ź������������ض���ʱ��
	float PlayAttackAction(EEnemyAttackType AttackType);

	//�������˶������ض���ʱ��
	float PlayHurtAction();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float IdleType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		FVector RootBonePose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float RootBoneAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		bool IsDefence;

protected:
	//�����ɫ
	class ASlAiEnemyCharacter* SECharacter;

	//�ȴ�����ָ��
	class UAnimSequence* AnimIdle_I;
	class UAnimSequence* AnimIdle_II;
	class UAnimSequence* AnimIdle_III;

	//��������ָ��
	class UAnimMontage* AnimAttack_I;
	class UAnimMontage* AnimAttack_II;
	class UAnimMontage* AnimAttack_III;
	class UAnimMontage* AnimAttack_IV;

	class UAnimSequence* AnimAttackSeq_III;
	class UAnimSequence* AnimAttackSeq_IV;

	class UAnimMontage* AnimHurt;

	//������ʱ��
	float CurrentPlayTime;
	//������һ��Y��λ��
	float StartYPos;
};
