// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "SlAiTypes.h"
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

	//�����͹ر�������Ʒ�Ľ������
	UFUNCTION(BlueprintCallable, Category = "SlAi")
		void ChangeDetection(bool IsOpen);

	//�����͹ر�������Ʒ����ʾ����ڼ�����ʱ�����
	UFUNCTION(BlueprintCallable, Category = "PlayAnim")
		void RenderHandObject(bool IsRender);

	//���궫������
	UFUNCTION(BlueprintCallable, Category = "PlayAnim")
		void EatUpEvent();

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

	//�޸��Ƿ������л��ӽ�
	void AllowViewChange(bool IsAllow);

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

	//�ƶ��Լ��������˳�
	EGameViewMode::Type GameView;
};
