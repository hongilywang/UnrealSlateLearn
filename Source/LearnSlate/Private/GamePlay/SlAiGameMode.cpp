// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameMode.h"

#include "SlAiDataHandle.h"
#include "SlAiHelper.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiGameInstance.h"

ASlAiGameMode::ASlAiGameMode()
{

}

void ASlAiGameMode::BeginPlay()
{
	SlAiHelper::Debug(FString("DataHandle: ") + SlAiDataHandle::Get()->RecordName, 30.f);
	SlAiHelper::Debug(FString("DataHandle: ") + Cast<USlAiGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GameName, 30.f);
}
