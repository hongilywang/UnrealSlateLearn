// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPlayerController.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiTypes.h"
#include "SlAiPlayerState.h"
#include "SlAiHandObject.h"
#include "CollisionQueryParams.h"
#include "Components/LineBatchComponent.h"
#include "Camera/CameraComponent.h"
#include "SlAiPickupObject.h"
#include "SlAiResourceObject.h"
#include "SlAiEnemyCharacter.h"
#include "TimerManager.h"

ASlAiPlayerController::ASlAiPlayerController()
{

}

void ASlAiPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//���߼��
	RunRayCast();

	//������״̬
	StateMachine();

	//����С��ͼ����
	TickMiniMap();
}

void ASlAiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//���ӽ��л�
	InputComponent->BindAction("ChangeView", IE_Pressed, this, &ASlAiPlayerController::ChangeView);
	//����갴���¼�
	InputComponent->BindAction("LeftEvent", IE_Pressed, this, &ASlAiPlayerController::LeftEventStart);
	InputComponent->BindAction("LeftEvent", IE_Released, this, &ASlAiPlayerController::LeftEventStop);
	InputComponent->BindAction("RightEvent", IE_Pressed, this, &ASlAiPlayerController::RightEventStart);
	InputComponent->BindAction("RightEvent", IE_Released, this, &ASlAiPlayerController::RightEventStop);
	//�󶨻��ֹ����¼�
	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &ASlAiPlayerController::ScrollUpEvent);
	InputComponent->BindAction("ScrollDown", IE_Released, this, &ASlAiPlayerController::ScrollDownEvent);

	//��Esc�¼�����ͣ��Ϸ,���Ҹ��¼�����Ϸ��ͣ��ʱ����Ȼ��������
	InputComponent->BindAction("EscEvent", IE_Pressed, this, &ASlAiPlayerController::EscEvent).bExecuteWhenPaused = true;
	//�󶨱���
	InputComponent->BindAction("PackageEvent", IE_Pressed, this, &ASlAiPlayerController::PackageEvent);
	//������
	InputComponent->BindAction("ChatRoomEvent", IE_Pressed, this, &ASlAiPlayerController::ChatRoomEvent);

	//������С��ͼ�¼�
	InputComponent->BindAction("AddMapSize", IE_Pressed, this, &ASlAiPlayerController::AddMapSizeStart);
	InputComponent->BindAction("AddMapSize", IE_Released, this, &ASlAiPlayerController::AddMapSizeStop);
	InputComponent->BindAction("ReduceMapSize", IE_Pressed, this, &ASlAiPlayerController::ReduceMapSizeStart);
	InputComponent->BindAction("ReduceMapSize", IE_Released, this, &ASlAiPlayerController::ReduceMapSizeStop);
}

void ASlAiPlayerController::ChangeHandObject()
{
	SPCharacter->ChangeHandObject(ASlAiHandObject::SpawnHandObject(SPState->GetCurrentHandObjectIndex()));
}

void ASlAiPlayerController::PlayerDead()
{
	//ת���������˳��ӽ�
	SPCharacter->ChangeView(EGameViewMode::Third);
	//
	float DeadDuration = SPCharacter->PlayDeadAnim();
	//��ס����
	LockedInput(true);
	//����¼�
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASlAiPlayerController::DeadTimeOut);
	//�ӳ���ʾUI
	GetWorld()->GetTimerManager().SetTimer(DeadHandle, TimerDelegate, DeadDuration, false);
}

FHitResult ASlAiPlayerController::RayGetHitResult(FVector TraceStart, FVector TraceEnd)
{
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(SPCharacter);
	//TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1, TraceParams))
	{
		//DrawRayLine(TraceStart, TraceEnd, 5.f);
	}

	return Hit;
}

void ASlAiPlayerController::DrawRayLine(FVector StartPos, FVector EndPos, float Duration)
{
	ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
	if (LineBatcher != nullptr)
	{
		float LineDurationTime = (Duration > 0.f) ? Duration : LineBatcher->DefaultLifeTime;
		LineBatcher->DrawLine(StartPos, EndPos, FLinearColor::Red, 10, 0.f, LineDurationTime);
	}
}

void ASlAiPlayerController::RunRayCast()
{
	FVector StartPos(0.f);
	FVector EndPos(0.f);

	switch (SPCharacter->GameView)
	{
	case EGameViewMode::First:
		StartPos = SPCharacter->FirstCamera->K2_GetComponentLocation();
		EndPos = StartPos + SPCharacter->FirstCamera->GetForwardVector() * 2000.f;
	case EGameViewMode::Third:
		StartPos = SPCharacter->ThirdCamera->K2_GetComponentLocation();
		StartPos = StartPos + SPCharacter->ThirdCamera->GetForwardVector() * 300.f;
		EndPos = StartPos + SPCharacter->ThirdCamera->GetForwardVector() * 2000.f;
		break;
	}

	//�Ƿ��⵽��Ʒ
	bool IsDetected = false;
	FHitResult Hit = RayGetHitResult(StartPos, EndPos);
	RayActor = Hit.GetActor();

	if (Cast<ASlAiPickupObject>(RayActor))
	{
		IsDetected = true;
		SPState->RayInfoText = Cast<ASlAiPickupObject>(RayActor)->GetInfoText();
	}
	else if (Cast<ASlAiResourceObject>(RayActor))
	{
		IsDetected = true;
		SPState->RayInfoText = Cast<ASlAiResourceObject>(RayActor)->GetInfoText();
	}
	else if (Cast<ASlAiEnemyCharacter>(RayActor))
	{
		IsDetected = true;
		SPState->RayInfoText = Cast<ASlAiEnemyCharacter>(RayActor)->GetInfoText();
	}

	if (!IsDetected)
	{
		SPState->RayInfoText = FText();
	}
}

void ASlAiPlayerController::StateMachine()
{
	//��ͨģʽ
	ChangePreUpperType(EUpperBody::None);
	if (!Cast<ASlAiResourceObject>(RayActor) && !Cast<ASlAiPickupObject>(RayActor) && !Cast<ASlAiEnemyCharacter>(RayActor))
	{
		//׼����ʾΪ����
		UpdatePointer.ExecuteIfBound(false, 1.f);
	}
	else if (Cast<ASlAiEnemyCharacter>(RayActor))
	{
		//׼����ʾΪ����
		UpdatePointer.ExecuteIfBound(false, 0.f);
	}
	else if (Cast<ASlAiResourceObject>(RayActor))
	{
		if (!IsLeftButtonDown)
		{
			//׼������ģʽ
			UpdatePointer.ExecuteIfBound(false, 0.f);
		}
		//�����⵽��Դ
		if (IsLeftButtonDown && FVector::Distance(RayActor->GetActorLocation(), SPCharacter->GetActorLocation()) < SPState->GetAffectRange())
		{
			//��ȡʵ���˺�
			int Damage = SPState->GetDamageValue(Cast<ASlAiResourceObject>(RayActor)->GetResourceType());
			//����Դ���˺�����ȡʣ��Ѫ���ٷֱ�
			float Range = Cast<ASlAiResourceObject>(RayActor)->TakeObjectDamage(Damage)->GetHPRange();
			//����׼��
			UpdatePointer.ExecuteIfBound(true, Range);
		}
	}
	//�����⵽��ʰ��Ʒ�����Ҿ���С��300
	else if (Cast<ASlAiPickupObject>(RayActor) && FVector::Distance(RayActor->GetActorLocation(), SPCharacter->GetActorLocation()) < 300.f)
	{
		//�ı��Ҽ�Ԥ״̬Ϊʰȡ
		ChangePreUpperType(EUpperBody::PickUp);
		//�޸�׼������ģʽ
		UpdatePointer.ExecuteIfBound(false, 0);
		//���Ҽ�����
		if (IsRightButtonDown && SPCharacter->IsPackageFree(Cast<ASlAiPickupObject>(RayActor)->ObjectIndex))
		{
			SPCharacter->AddPackageObject(Cast<ASlAiPickupObject>(RayActor)->TakePickup());
		}
	}
}

void ASlAiPlayerController::EscEvent()
{
	switch (CurrentUIType)
	{
	case EGameUIType::Game:
		//������Ϸ��ͣ
		SetPause(true);
		//��������ģʽΪGameAndUI
		SwitchInputMode(false);
		//���½���
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Pause);
		//���µ�ǰUI
		CurrentUIType = EGameUIType::Pause;
		//��������
		LockedInput(true);
		break;
	case EGameUIType::Pause:
	case EGameUIType::Package:
	case EGameUIType::ChatRoom:
		//�����ͣ
		SetPause(false);
		SwitchInputMode(true);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Game);
		CurrentUIType = EGameUIType::Game;
		//�⿪����
		LockedInput(false);
		break;
	}
}

void ASlAiPlayerController::PackageEvent()
{
	switch (CurrentUIType)
	{
	case EGameUIType::Game:
		SwitchInputMode(false);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Package);
		CurrentUIType = EGameUIType::Package;
		//��������
		LockedInput(true);
		break;
	case EGameUIType::Package:
		SwitchInputMode(true);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Game);
		CurrentUIType = EGameUIType::Game;
		//�⿪����
		LockedInput(false);
		break;
	}
}

void ASlAiPlayerController::ChatRoomEvent()
{
	switch (CurrentUIType)
	{
	case EGameUIType::Game:
		SwitchInputMode(false);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::ChatRoom);
		CurrentUIType = EGameUIType::ChatRoom;
		//��������
		LockedInput(true);
		break;
	case EGameUIType::ChatRoom:
		SwitchInputMode(true);
		ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Game);
		CurrentUIType = EGameUIType::Game;
		//�⿪����
		LockedInput(false);
		break;
	}
}

void ASlAiPlayerController::SwitchInputMode(bool IsGameOnly)
{
	if (IsGameOnly)
	{
		//�������
		bShowMouseCursor = false;
		//��������ģʽΪOnlyGame;
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(true);
		SetInputMode(InputMode);
	}
	else
	{
		//��ʾ���
		bShowMouseCursor = true;
		//��������ģʽΪGameAndUI;
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
}

void ASlAiPlayerController::LockedInput(bool IsLocked)
{
	SPCharacter->IsInputLocked = IsLocked;
}

void ASlAiPlayerController::AddMapSizeStart()
{
	//�����������ס��ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;
	MiniMapSizeMode = EMiniMapSizeMode::Add;
}

void ASlAiPlayerController::AddMapSizeStop()
{
	//�����������ס��ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;
	MiniMapSizeMode = EMiniMapSizeMode::None;
}

void ASlAiPlayerController::ReduceMapSizeStart()
{
	//�����������ס��ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;
	MiniMapSizeMode = EMiniMapSizeMode::Reduce;
}

void ASlAiPlayerController::ReduceMapSizeStop()
{
	//�����������ס��ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;
	MiniMapSizeMode = EMiniMapSizeMode::None;
}

void ASlAiPlayerController::TickMiniMap()
{
	switch (MiniMapSizeMode)
	{
	case EMiniMapSizeMode::Add:
		UpdateMiniMapWidth.ExecuteIfBound(5);
		break;
	case EMiniMapSizeMode::Reduce:
		UpdateMiniMapWidth.ExecuteIfBound(-5);
		break;
	}
}

void ASlAiPlayerController::DeadTimeOut()
{
	//������Ϸ��ͣ
	SetPause(true);
	//������ϷģʽΪ���
	SwitchInputMode(false);
	//���½���
	ShowGameUI.ExecuteIfBound(CurrentUIType, EGameUIType::Lose);
	//���µ�ǰUI
	CurrentUIType = EGameUIType::Lose;
	//��ס����
	LockedInput(true);
}

void ASlAiPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//��ȡ��ɫ
	if (!SPCharacter)
		SPCharacter = Cast<ASlAiPlayerCharacter>(GetCharacter());
	//��ȡ״̬
	if (!SPState)
		SPState = Cast<ASlAiPlayerState>(PlayerState);

	//����������벻��ʾ
	bShowMouseCursor = false;
	//��������ģʽ
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);

	//����Ԥ����
	LeftUpperType = EUpperBody::Punch;
	RightUpperType = EUpperBody::PickUp;
	
	IsLeftButtonDown = false;
	IsRightButtonDown = false;

	CurrentUIType = EGameUIType::Game;

	MiniMapSizeMode = EMiniMapSizeMode::None;
}

void ASlAiPlayerController::ChangeView()
{
	//�����ס������ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;

	//����������л��ӽ�
	if (!SPCharacter->IsAllowSwitch)
		return;

	switch (SPCharacter->GameView)
	{
	case EGameViewMode::First:
		SPCharacter->ChangeView(EGameViewMode::Third);
		break;
	case EGameViewMode::Third:
		SPCharacter->ChangeView(EGameViewMode::First);
		break;
	}
}

void ASlAiPlayerController::LeftEventStart()
{
	//�����ס������ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;

	IsLeftButtonDown = true;
	SPCharacter->UpperType = LeftUpperType;
}

void ASlAiPlayerController::LeftEventStop()
{
	//�����ס������ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;

	IsLeftButtonDown = false;
	SPCharacter->UpperType = EUpperBody::None;
}

void ASlAiPlayerController::RightEventStart()
{
	//�����ס������ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;

	IsRightButtonDown = true;
	SPCharacter->UpperType = RightUpperType;
}

void ASlAiPlayerController::RightEventStop()
{
	//�����ס������ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;

	IsRightButtonDown = false;
	SPCharacter->UpperType = EUpperBody::None;
}

void ASlAiPlayerController::ScrollUpEvent()
{
	//�����ס������ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;

	//����������л���ֱ�ӷ���
	if (!SPCharacter->IsAllowSwitch)
		return;
	
	if (IsLeftButtonDown || IsRightButtonDown)
		return;

	//����״̬���л����������
	SPState->ChooseShortcut(true);
	ChangeHandObject();
}

void ASlAiPlayerController::ScrollDownEvent()
{
	//�����ס������ֱ�ӷ���
	if (SPCharacter->IsInputLocked)
		return;

	//����������л���ֱ�ӷ���
	if (!SPCharacter->IsAllowSwitch)
		return;

	if (IsLeftButtonDown || IsRightButtonDown)
		return;

	//����״̬���л����������
	SPState->ChooseShortcut(false);
	ChangeHandObject();
}

void ASlAiPlayerController::ChangePreUpperType(EUpperBody::Type RightType = EUpperBody::None)
{
	//���ݵ�ǰ�ֳ���Ʒ���������޸�Ԥ����
	switch (SPState->GetCurrentObjectType())
	{
	case EObjectType::Normal:
		LeftUpperType = EUpperBody::Punch;
		RightUpperType = RightType;
		break;
	case EObjectType::Food:
		LeftUpperType = EUpperBody::Eat;
		RightUpperType = RightType == EUpperBody::None ? EUpperBody::Eat : RightType;
		break;
	case EObjectType::Tool:
		LeftUpperType = EUpperBody::Hit;
		RightUpperType = RightType;
		break;
	case EObjectType::Weapon:
		LeftUpperType = EUpperBody::Fight;
		RightUpperType = RightType;
		break;
	}
}
