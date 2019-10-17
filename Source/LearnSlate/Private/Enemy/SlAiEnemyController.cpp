// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiEnemyCharacter.h"
#include "Camera/CameraComponent.h"

#include "SlAiEnemyBlackboard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "TimerManager.h"

ASlAiEnemyController::ASlAiEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlAiEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//��ȡ��ɫ
	SECharacter = Cast<ASlAiEnemyCharacter>(InPawn);

	//��ȡ��Ϊ����Դ
	UBehaviorTree* StaticBehaviorTreeObject = LoadObject<UBehaviorTree>(nullptr, TEXT("BehaviorTree'/Game/Blueprint/Enemy/EnemyBehaviorTree.EnemyBehaviorTree'"));

	UBehaviorTree* BehaviorTreeObject = DuplicateObject<UBehaviorTree>(StaticBehaviorTreeObject, nullptr);

	//�ж϶���Դ�Ƿ����
	if (!BehaviorTreeObject)
		return;

	BehaviorTreeObject->BlackboardAsset = DuplicateObject<USlAiEnemyBlackboard>((USlAiEnemyBlackboard*)StaticBehaviorTreeObject->BlackboardAsset, nullptr);

	BlackBoardComp = Blackboard;

	bool IsSuccess = true;

	//����Դ �ο�����ӿ�->RunBehaviorTree()
	if (BehaviorTreeObject->BlackboardAsset && (Blackboard == nullptr || Blackboard->IsCompatibleWith(BehaviorTreeObject->BlackboardAsset) == false))
	{
		IsSuccess = UseBlackboard(BehaviorTreeObject->BlackboardAsset, BlackBoardComp);
	}

	if (IsSuccess)
	{
		BehaviorComp = Cast<UBehaviorTreeComponent>(BrainComponent);
		if (!BehaviorComp)
		{
			BehaviorComp = NewObject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
			BehaviorComp->RegisterComponent();
		}
		BrainComponent = BehaviorComp;
		check(BehaviorComp != nullptr);
		BehaviorComp->StartTree(*BehaviorTreeObject, EBTExecutionMode::Looped);

		//����Ԥ״̬ΪѲ��
		BlackBoardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Patrol);
		//Ҳ����ͨ�����ַ�ʽ����
		/*int32 EnemyStateIndex = BlackBoardComp->GetKeyID("EnemyState");
		BlackBoardComp->SetValue<UBlackboardKeyType_Enum>(EnemyStateIndex, (UBlackboardKeyType_Enum::FDataType)EEnemyAIState::ES_Patrol);*/

		//�޸ĵ��˵ĳ�ʼ�ƶ��ٶ�100
		SECharacter->SetMaxSpeed(100.f);
	}
}

void ASlAiEnemyController::OnUnPossess()
{
	Super::OnUnPossess();

	//ֹͣ��Ϊ��
	if (BehaviorComp)
		BehaviorComp->StopTree();
}

void ASlAiEnemyController::BeginPlay()
{
	//���ø��ຯ��
	Super::BeginPlay();

	//
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!SECharacter)
		SECharacter = Cast<ASlAiEnemyCharacter>(GetPawn());
	//��ʼ����δ�������
	IsLockPlayer = false;
	//����ί�а�
	FTimerDelegate EPDisDele = FTimerDelegate::CreateUObject(this, &ASlAiEnemyController::UpdateStatePama);
	GetWorld()->GetTimerManager().SetTimer(EPDisHandle, EPDisDele, 0.3f, true);
}

void ASlAiEnemyController::UpdateStatePama()
{
	//��������ҵľ�������
	if (EPDisList.Num() < 6)
	{
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}
	else
	{
		EPDisList.RemoveAt(0);
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}
}

void ASlAiEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ÿ֡����Ѫ����λ�ó���
	if (SECharacter && SPCharacter && SPCharacter->GetCurrentCamera())
		SECharacter->UpdateHPBarRotation(SPCharacter->GetCurrentCamera()->GetComponentLocation());
}

FVector ASlAiEnemyController::GetPlayerLocation() const
{
	//������ָ����ڣ��������λ��
	if (SPCharacter)
		return SPCharacter->GetActorLocation();

	return FVector::ZeroVector;
}

bool ASlAiEnemyController::IsPlayerDead()
{
	if (SPCharacter)
		return SPCharacter->IsPlayerDead();
	return false;
}

void ASlAiEnemyController::OnSeePlayer()
{
	//����Ѿ���������һ����������������ִ������ĺ���
	if (IsLockPlayer || IsPlayerDead())
		return;

	//�����������
	IsLockPlayer = true;
	//�޸�Ԥ״̬Ϊ׷��
	BlackBoardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Chase);
	SECharacter->SetMaxSpeed(300.f);
}

void ASlAiEnemyController::LoosePlayer()
{
	IsLockPlayer = false;
}

bool ASlAiEnemyController::IsPlayerAway()
{
	if (!IsLockPlayer || !SPCharacter || EPDisList.Num() < 6 || IsPlayerDead())
	{
		return false;
	}

	int BiggerNum = 0;
	float LastDis = -1.f;
	//ֻҪż3����ǰ��Ĵ��ж�Զ��
	for (TArray<float>::TIterator It(EPDisList); It; ++It)
	{
		if (*It < LastDis)
			BiggerNum += 1;
		LastDis = *It;
	}
	return BiggerNum > 3;
}

UObject* ASlAiEnemyController::GetPlayerPawn()
{
	return SPCharacter;
}

