// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiThirdPlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "SlAiPlayerCharacter.h"

USlAiThirdPlayerAnim::USlAiThirdPlayerAnim()
{
	Direction = 0.f;
	IsInAir = false;
}

void USlAiThirdPlayerAnim::UpdateParameter()
{
	Super::UpdateParameter();

	if (!SPCharacter)
		return;

	//��ȡ�Ƿ�����Ծ
	IsInAir = SPCharacter->GetCharacterMovement()->IsFalling();
	//�ٶȳ����yaw��ת��ȥActor������ʵ������������yaw��ת�õ���Եķ���
	float PreDir = SPCharacter->GetVelocity().ToOrientationRotator().Yaw - SPCharacter->GetActorRotation().Yaw;
	if (PreDir > 180.f)
		PreDir -= 360.f;
	if (PreDir < -180.f)
		PreDir += 360.f;
	Direction = PreDir;
}
