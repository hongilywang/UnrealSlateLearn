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

	if (!Montage_IsPlaying(PlayerPunchMontage))
	{
		Montage_Play(PlayerPunchMontage);
	}
}
