// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlAiTypes.h"
#include "SlAiEnemyCharacter.generated.h"

UCLASS()
class LEARNSLATE_API ASlAiEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlAiEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//生成掉落物
	void CreateFlobObject();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//实时更新血条的朝向， 由controller调用，传入玩家位置
	void UpdateHPBarRotation(FVector SPLocation);

	//修改移动速度
	void SetMaxSpeed(float Speed);

	//获取等待时长
	float GetIdleWaitTime();

	//播放攻击动画，返回攻击时长
	float PlayAttackAction(EEnemyAttackType AttackType);

	//接受攻击，可以重写APawn的TaskDamage函数
	void AcceptDamage(int DamageValue);

	//播放受伤动画
	float PlayHurtAction();

	//开启防御
	void StartDefence();

	//停止防御
	void StopDefence();

	//销毁函数
	void DestroyEvent();

	//获取物品信息
	FText GetInfoText() const;

	//修改手持物品的碰撞检测是否开启
	void ChangeWeaponDetect(bool IsOpen);

	//获取是否已经锁定了玩家
	bool IsLockPlayer();

	//加载血量
	void LoadHP(float HPValue);

public:
	//资源ID
	int ResourceIndex;

	//是否在下一帧销毁自己，由GameMode加载游戏存档时进行设置
	bool IsDestroyNextTick;

protected:
	//武器插槽
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* WeaponSocket;

	//赌牌插槽
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* SheildSocket;

	//血条
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UWidgetComponent* HPBar;

	//敌人感知
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UPawnSensingComponent* EnemySense;

private:
	//绑定到敌人感知的方法
	UFUNCTION()
		void OnSeePlayer(APawn* PlayerChar);

	//血条UI引用
	TSharedPtr<class SSlAiEnemyHPWidget> HPBarWidget;

	//控制器引用
	class ASlAiEnemyController* SEController;

	//血量
	float HP;

	//动作引用
	class USlAiEnemyAnim* SEAnim;

	//死亡动画资源
	class UAnimationAsset* AnimDead_I;
	class UAnimationAsset* AnimDead_II;

	//死亡时间委托
	FTimerHandle DeadHandle;
};
