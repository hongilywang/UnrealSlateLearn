// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiThirdPlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "SlAiPlayerCharacter.h"
#include "Animation/AnimMontage.h"
#include "ConstructorHelpers.h"

USlAiThirdPlayerAnim::USlAiThirdPlayerAnim()
{
	//绑定资源到montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerHitMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerHitMontage.PlayerHitMontage'"));
	PlayerHitMontage = PlayerHitMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerEatMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerEatMontage.PlayerEatMontage'"));
	PlayerEatMontage = PlayerEatMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerFightMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerFightMontage.PlayerFightMontage'"));
	PlayerFightMontage = PlayerFightMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPunchMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerPunchMontage.PlayerPunchMontage'"));
	PlayerPunchMontage = PlayerPunchMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPickUpMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerPickUpMontage.PlayerPickUpMontage'"));
	PlayerPickUpMontage = PlayerPickUpMon.Object;

	//设置运行时的人称视角
	GameView = EGameViewMode::Third;

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
