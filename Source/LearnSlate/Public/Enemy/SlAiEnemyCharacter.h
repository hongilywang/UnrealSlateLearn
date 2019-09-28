// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ʵʱ����Ѫ���ĳ��� ��controller���ã��������λ��
	void UpdateHPBarRotation(FVector SPLocation);

protected:
	//�������
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* WeaponSocket;

	//���Ʋ��
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* SheildSocket;

	//Ѫ��
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UWidgetComponent* HPBar;

	//���˸�֪
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UPawnSensingComponent* EnemySense;

private:
	//�󶨵����˸�֪�ķ���
	UFUNCTION()
		void OnSeePlayer(APawn* PlayerChar);

	//Ѫ��UI����
	TSharedPtr<class SSlAiEnemyHPWidget> HPBarWidget;

	//����������
	class ASlAiEnemyController* SEController;

	//Ѫ��
	float HP;
};
