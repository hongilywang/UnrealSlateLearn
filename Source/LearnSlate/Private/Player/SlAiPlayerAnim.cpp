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
	//初始化角色指针
	InitSPCharacter();
	//更新蓝图数据
	UpdateParameter();
	//跟新动作
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

	//如果当前的人称状态和这个动作的不一致，直接返回
	if (SPCharacter->GameView != GameView)
		return;

	//如果当前的动作没有停止，不更新动作
	if (!Montage_GetIsStopped(CurrentMontage))
		return;

	switch (SPCharacter->UpperType)
	{
	case EUpperBody::None:
		//如果现在有动作在播放
		if (CurrentMontage != nullptr)
		{
			Montage_Stop(0);
			CurrentMontage = nullptr;
			//允许切换视角
			AllowViewChange(true);
		}
		break;
	case EUpperBody::Punch:
		if (!Montage_IsPlaying(PlayerPunchMontage))
		{
			Montage_Play(PlayerPunchMontage);
			CurrentMontage = PlayerPunchMontage;
			//不允许切换视角
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Hit:
		if (!Montage_IsPlaying(PlayerHitMontage))
		{
			Montage_Play(PlayerHitMontage);
			CurrentMontage = PlayerHitMontage;
			//不允许切换视角
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Fight:
		if (!Montage_IsPlaying(PlayerFightMontage))
		{
			Montage_Play(PlayerFightMontage);
			CurrentMontage = PlayerFightMontage;
			//不允许切换视角
			AllowViewChange(false);
		}
		break;
	case EUpperBody::PickUp:
		if (!Montage_IsPlaying(PlayerPickUpMontage))
		{
			Montage_Play(PlayerPickUpMontage);
			CurrentMontage = PlayerPickUpMontage;
			//不允许切换视角
			AllowViewChange(false);
		}
		break;
	case EUpperBody::Eat:
		if (!Montage_IsPlaying(PlayerEatMontage))
		{
			Montage_Play(PlayerEatMontage);
			CurrentMontage = PlayerEatMontage;
			//不允许切换视角
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
