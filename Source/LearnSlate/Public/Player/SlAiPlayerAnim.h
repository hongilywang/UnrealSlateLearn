// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "SlAiPlayerAnim.generated.h"

class UAnimMontage;
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

	//���¶���
	virtual void UpdateMontage();

protected:

	//��ɫָ��
	class ASlAiPlayerCharacter* SPCharacter;

	//�ϰ����Montage
	UAnimMontage* PlayerHitMontage;
	UAnimMontage* PlayerEatMontage;
	UAnimMontage* PlayerFightMontage;
	UAnimMontage* PlayerPunchMontage;
	UAnimMontage* PlayerPickUpMontage;

	//���浱ǰ���ŵ�Montage
	UAnimMontage* CurrentMontage;
};
