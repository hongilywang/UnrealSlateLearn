// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPlayerAnim.h"
#include "SlAiPlayerCharacter.h"

USlAiPlayerAnim::USlAiPlayerAnim()
{
	Speed = 0.f;
	SpineRotator = FRotator(0.f, 0.f, 0.f);
}

void USlAiPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	//��ʼ����ɫָ��
	InitSPCharacter();
	//������ͼ����
	UpdateParameter();
	//���¶���
	UpdateMontage();
}

void USlAiPlayerAnim::InitSPCharacter()
{
	if (!SPCharacter)
		SPCharacter = Cast<ASlAiPlayerCharacter>(TryGetPawnOwner());
}

void USlAiPlayerAnim::UpdateParameter()
{
	if (!SPCharacter)
		return;

	Speed = SPCharacter->GetVelocity().Size();
}

void USlAiPlayerAnim::UpdateMontage()
{
	if (!SPCharacter)
		return;

	//�����ǰ���˳�״̬����������Ĳ�һ�£�ֱ�ӷ���
	if (SPCharacter->GameView != GameView)
		return;

	//�����ǰ�Ķ���û��ֹͣ�������¶���
	if (!Montage_GetIsStopped(CurrentMontage))
		return;

	switch (SPCharacter->UpperType)
	{
	case EUpperBody::None:
		//��������ж����ڲ���
		if (CurrentMontage != nullptr)
		{
			Montage_Stop(0);
			CurrentMontage = nullptr;
			//�����л��ӽ�
			AllowViewChange(true);
		}
		break;
	case EUpperBody::Punch:
		if (!Montage_IsPlaying(PlayerPunchMontage))
		{
			Montage_Play(PlayerPunchMontage);
			CurrentMontage = PlayerPunchMontage;
			//�������л��ӽ�
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Hit:
		if (!Montage_IsPlaying(PlayerHitMontage))
		{
			Montage_Play(PlayerHitMontage);
			CurrentMontage = PlayerHitMontage;
			//�������л��ӽ�
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Fight:
		if (!Montage_IsPlaying(PlayerFightMontage))
		{
			Montage_Play(PlayerFightMontage);
			CurrentMontage = PlayerFightMontage;
			//�������л��ӽ�
			AllowViewChange(false);
		}
		break;
	case EUpperBody::PickUp:
		if (!Montage_IsPlaying(PlayerPickUpMontage))
		{
			Montage_Play(PlayerPickUpMontage);
			CurrentMontage = PlayerPickUpMontage;
			//�������л��ӽ�
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Eat:
		if (!Montage_IsPlaying(PlayerEatMontage))
		{
			Montage_Play(PlayerEatMontage);
			CurrentMontage = PlayerEatMontage;
			//�������л��ӽ�
			AllowViewChange(false);
		}
		break;
	}
}

void USlAiPlayerAnim::AllowViewChange(bool IsAllow)
{
	if (!SPCharacter)
		return;
	SPCharacter->IsAllowSwitch = IsAllow;
}
