// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiFlobObject.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"
#include "../Player/SlAiPlayerCharacter.h"

// Sets default values
ASlAiFlobObject::ASlAiFlobObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = (USceneComponent*)BoxCollision;
	//������ײ����
	BoxCollision->SetCollisionProfileName(FName("FlobProfile"));
	//��������ģ��
	BoxCollision->SetSimulatePhysics(true);
	//������ת
	BoxCollision->SetConstraintMode(EDOFMode::Default);
	BoxCollision->GetBodyInstance()->bLockXRotation = true;
	BoxCollision->GetBodyInstance()->bLockYRotation = true;
	BoxCollision->GetBodyInstance()->bLockZRotation = true;
	//���ô�С
	BoxCollision->SetBoxExtent(FVector(15.f));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);
	BaseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//���ñ任
	BaseMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	BaseMesh->SetRelativeScale3D(FVector(0.3f));

	UMaterialInterface* StaticObjectIconMat = LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/FlobIconMatInst.FlobIconMatInst'"));
	//��̬��������
	ObjectIconMatDynamic = UMaterialInstanceDynamic::Create(StaticObjectIconMat, nullptr);
}

// Called when the game starts or when spawned
void ASlAiFlobObject::BeginPlay()
{
	Super::BeginPlay();
	
	//��������Ƿ����
	if (!GetWorld())
		return;
	
	//ע�����¼�
	FTimerDelegate DetectPlayerDele;
	DetectPlayerDele.BindUObject(this, &ASlAiFlobObject::DetectPlayer);
	//ÿ������һ�Σ�ѭ�����У��ӳ�3������
	GetWorld()->GetTimerManager().SetTimer(DetectTimer, DetectPlayerDele, 1.f, true, 3.f);
	//ע�������¼�
	FTimerDelegate DestroyDele;
	DestroyDele.BindUObject(this, &ASlAiFlobObject::DestroyEvent);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, DestroyDele, 30.f, false);

	SPCharacter = nullptr;
}

void ASlAiFlobObject::RenderTexture()
{
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
	ObjectIconTex = LoadObject<UTexture>(nullptr, *ObjectAttr->TexPath);
	ObjectIconMatDynamic->SetTextureParameterValue(FName("ObjectTex"), ObjectIconTex);
	BaseMesh->SetMaterial(0, ObjectIconMatDynamic);
}

void ASlAiFlobObject::DetectPlayer()
{
	if (!GetWorld())
		return;

	//��������
	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	//bTraceAysncScene�Ѿ���4.22�����׸ɵ���
	//Params.bTraceAsyncScene = true;
	
	//���ж�̬��⣬��ⷶΧ��200,
	if (GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(200.f), Params))
	{
		for (TArray<FOverlapResult>::TIterator It(Overlaps); It; ++It)
		{
			//�����⵽�����
			if (Cast<ASlAiPlayerCharacter>(It->GetActor()))
			{
				SPCharacter = Cast<ASlAiPlayerCharacter>(It->GetActor());
				//�����ռ��ж�
				if (SPCharacter->IsPackageFree(ObjectIndex))
				{
					//ֹͣ��ⶨʱ��
					GetWorld()->GetTimerManager().PauseTimer(DetectTimer);
					//ֹͣ���ٶ�ʱ��
					GetWorld()->GetTimerManager().PauseTimer(DestroyTimer);
					//�ر�����ģ��
					BoxCollision->SetSimulatePhysics(false);
				}
				return;
			}
		}
	}
}

void ASlAiFlobObject::DestroyEvent()
{
	if (!GetWorld())
		return;
	
	//ע����ʱ��
	GetWorld()->GetTimerManager().ClearTimer(DetectTimer);
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
	//�����Լ�
	GetWorld()->DestroyActor(this);
}

// Called every frame
void ASlAiFlobObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//һֱ��ת
	BaseMesh->AddLocalRotation(FRotator(DeltaTime * 60.f, 0.f, 0.f));
	//�����⵽���
	if (SPCharacter)
	{
		//�������
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), SPCharacter->GetActorLocation() + FVector(0.f, 0.f, 40.f), DeltaTime, 5.f));
		//�������ӽ�
		if (FVector::Distance(GetActorLocation(), SPCharacter->GetActorLocation() + FVector(0.f, 0.f, 40.f)) < 10.f)
		{
			//�ж���ұ����Ƿ��пռ�
			if (SPCharacter->IsPackageFree(ObjectIndex))
			{
				//��Ӷ�Ӧ����Ʒ������
				SPCharacter->AddPackageObject(ObjectIndex);
				//�����Լ�
				DestroyEvent();
			}
			else
			{
				SPCharacter = nullptr;
				//������ⶨʱ��
				GetWorld()->GetTimerManager().UnPauseTimer(DetectTimer);
				//�������ٶ�ʱ��
				GetWorld()->GetTimerManager().UnPauseTimer(DestroyTimer);
				//��������ģ��
				BoxCollision->SetSimulatePhysics(true);
			}
		}
	}
}

void ASlAiFlobObject::CreateFlobObject(int ObjectID)
{
	//ָ��ID
	ObjectIndex = ObjectID;

	//��Ⱦ��ͼ
	RenderTexture();

	//������������
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int DirYaw = Stream.RandRange(-180, 180);
	FRotator ForceRot = FRotator(0.f, DirYaw, 0.f);

	//�����
	BoxCollision->AddForce((FVector(0.f, 0.f, 4.f) + ForceRot.Vector()) * 100000.f);
}

void ASlAiFlobObject::ThrowFlobObject(int ObjectID, float DirYaw)
{
	//ָ��ID
	ObjectIndex = ObjectID;

	//��Ⱦ��ͼ
	RenderTexture();

	//������������
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	DirYaw += Stream.RandRange(-30, 30);
	FRotator ForceRot = FRotator(0.f, DirYaw, 0.f);

	//�����
	BoxCollision->AddForce((FVector(0.f, 0.f, 2.f) + ForceRot.Vector()) * 120000.f);
}

