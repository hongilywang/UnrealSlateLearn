// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlAiDataHandle.h"
#include "SlAiFlobObject.generated.h"

UCLASS()
class LEARNSLATE_API ASlAiFlobObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlAiFlobObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//��ʼ��
	void CreateFlobObject(int ObjectID);

	//������Ʒ��ʼ������������ƷID�Ͷ�������
	void ThrowFlobObject(int ObjectID, float DirYaw);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//��Ⱦ��ͼ
	void RenderTexture();

	//��̬����¼�
	void DetectPlayer();

	//�����¼�
	void DestroyEvent();
		
private:
	class UBoxComponent* BoxCollision;
	class UStaticMeshComponent* BaseMesh;

	//��ƷID
	int ObjectIndex;

	class UTexture* ObjectIconTex;
	class UMaterialInstanceDynamic* ObjectIconMatDynamic;

	//���ָ��
	class ASlAiPlayerCharacter* SPCharacter;

	//��̬���Timer
	FTimerHandle DetectTimer;

	//����Timer
	FTimerHandle DestroyTimer;
};
