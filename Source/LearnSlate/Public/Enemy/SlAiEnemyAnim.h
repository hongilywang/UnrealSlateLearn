// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float IdleType;

protected:
	//�����ɫ
	class ASlAiEnemyCharacter* SECharacter;

	//�ȴ�����ָ��
	class UAnimSequence* AnimIdle_I;
	class UAnimSequence* AnimIdle_II;
	class UAnimSequence* AnimIdle_III;

};
