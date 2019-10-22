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

	//设置Idle模式，返回动作时长
	float SetIdelType(int NewType);

	//播放攻击动画，返回动画时长
	float PlayAttackAction(EEnemyAttackType AttackType);

	//播放受伤动画返回动画时长
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
	//保存角色
	class ASlAiEnemyCharacter* SECharacter;

	//等待动作指针
	class UAnimSequence* AnimIdle_I;
	class UAnimSequence* AnimIdle_II;
	class UAnimSequence* AnimIdle_III;

	//攻击动画指针
	class UAnimMontage* AnimAttack_I;
	class UAnimMontage* AnimAttack_II;
	class UAnimMontage* AnimAttack_III;
	class UAnimMontage* AnimAttack_IV;

	class UAnimSequence* AnimAttackSeq_III;
	class UAnimSequence* AnimAttackSeq_IV;

	class UAnimMontage* AnimHurt;

	//动作计时器
	float CurrentPlayTime;
	//动作第一针Y轴位置
	float StartYPos;
};
