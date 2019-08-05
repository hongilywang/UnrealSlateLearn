// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPlayerController.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiTypes.h"
#include "SlAiPlayerState.h"
#include "SlAiHandObject.h"

ASlAiPlayerController::ASlAiPlayerController()
{

}

void ASlAiPlayerController::Tick(float DeltaSeconds)
{
	//允许
	PrimaryActorTick.bCanEverTick = true;
}

void ASlAiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//绑定视角切换
	InputComponent->BindAction("ChangeView", IE_Pressed, this, &ASlAiPlayerController::ChangeView);
	//绑定鼠标按下事件
	InputComponent->BindAction("LeftEvent", IE_Pressed, this, &ASlAiPlayerController::LeftEventStart);
	InputComponent->BindAction("LeftEvent", IE_Released, this, &ASlAiPlayerController::LeftEventStop);
	InputComponent->BindAction("RightEvent", IE_Pressed, this, &ASlAiPlayerController::RightEventStart);
	InputComponent->BindAction("RightEvent", IE_Released, this, &ASlAiPlayerController::RightEventStop);
	//绑定滑轮滚动事件
	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &ASlAiPlayerController::ScrollUpEvent);
	InputComponent->BindAction("ScrollDown", IE_Released, this, &ASlAiPlayerController::ScrollDownEvent);
}

void ASlAiPlayerController::ChangeHandObject()
{
	SPCharacter->ChangeHandObject(ASlAiHandObject::SpawnHandObject(SPState->GetCurrentHandObjectIndex()));
}

void ASlAiPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//获取角色
	if (!SPCharacter)
		SPCharacter = Cast<ASlAiPlayerCharacter>(GetCharacter());
	//获取状态
	if (!SPState)
		SPState = Cast<ASlAiPlayerState>(PlayerState);

	//设置鼠标输入不显示
	bShowMouseCursor = false;
	//设置输入模式
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);

	//设置预动作
	LeftUpperType = EUpperBody::Punch;
	RightUpperType = EUpperBody::PickUp;
	
	IsLeftButtonDown = false;
	IsRightButtonDown = false;
}

void ASlAiPlayerController::ChangeView()
{
	//如果不允许切换视角
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
	IsLeftButtonDown = true;
	SPCharacter->UpperType = LeftUpperType;
}

void ASlAiPlayerController::LeftEventStop()
{
	IsLeftButtonDown = false;
	SPCharacter->UpperType = EUpperBody::None;
}

void ASlAiPlayerController::RightEventStart()
{
	IsRightButtonDown = true;
	SPCharacter->UpperType = RightUpperType;
}

void ASlAiPlayerController::RightEventStop()
{
	IsRightButtonDown = false;
	SPCharacter->UpperType = EUpperBody::None;
}

void ASlAiPlayerController::ScrollUpEvent()
{
	//如果不允许切换，直接返回
	if (!SPCharacter->IsAllowSwitch)
		return;
	
	if (IsLeftButtonDown || IsRightButtonDown)
		return;

	//告诉状态类切换快捷栏容器
	SPState->ChooseShortcut(true);
	ChangeHandObject();
}

void ASlAiPlayerController::ScrollDownEvent()
{
	//如果不允许切换，直接返回
	if (!SPCharacter->IsAllowSwitch)
		return;

	if (IsLeftButtonDown || IsRightButtonDown)
		return;

	//告诉状态类切换快捷栏容器
	SPState->ChooseShortcut(false);
	ChangeHandObject();
}
