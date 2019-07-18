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
	//��ȡ��ɫָ��
	void InitSPCharacter();

	//��������
	virtual void UpdateParameter();

protected:

	//��ɫָ��
	class ASlAiPlayerCharacter* SPCharacter;
};
