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

	//获取是否在跳跃
	IsInAir = SPCharacter->GetCharacterMovement()->IsFalling();
	//速度朝向的yaw旋转减去Actor朝向其实就是相机朝向的yaw旋转得到相对的方向
	float PreDir = SPCharacter->GetVelocity().ToOrientationRotator().Yaw - SPCharacter->GetActorRotation().Yaw;
	if (PreDir > 180.f)
		PreDir -= 360.f;
	if (PreDir < -180.f)
		PreDir += 360.f;
	Direction = PreDir;
}
