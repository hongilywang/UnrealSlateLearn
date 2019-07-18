// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlAiPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class LEARNSLATE_API USlAiPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USlAiPlayerAnim();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		FRotator SpineRotator;

protected:
	//获取角色指针
	void InitSPCharacter();

	//更新属性
	virtual void UpdateParameter();

protected:

	//角色指针
	class ASlAiPlayerCharacter* SPCharacter;
};
