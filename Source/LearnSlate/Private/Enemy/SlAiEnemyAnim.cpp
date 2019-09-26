// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyAnim.h"
#include "SlAiEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimSequence.h"
#include "ConstructorHelpers.h"

USlAiEnemyAnim::USlAiEnemyAnim()
{
	//��ʼ��
	Speed = 0.f;
	IdleType = 0.f;
}

void USlAiEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	//��ʼ����ɫָ��
	if (!SECharacter)
		SECharacter = Cast<ASlAiEnemyCharacter>(TryGetPawnOwner());
	if (!SECharacter)
		return;

	//�����ٶ�
	Speed = SECharacter->GetVelocity().Size();
}
