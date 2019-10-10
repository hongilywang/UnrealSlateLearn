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

	//设置Idle模式，返回动作时长
	float SetIdelType(int NewType);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float IdleType;

protected:
	//保存角色
	class ASlAiEnemyCharacter* SECharacter;

	//等待动作指针
	class UAnimSequence* AnimIdle_I;
	class UAnimSequence* AnimIdle_II;
	class UAnimSequence* AnimIdle_III;

};
