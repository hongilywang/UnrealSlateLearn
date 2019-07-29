// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPlayerState.h"
#include "STextBlock.h"

ASlAiPlayerState::ASlAiPlayerState()
{

}

void ASlAiPlayerState::RegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* ContainerList, TSharedPtr<STextBlock> ShortcutInfoTextBlock)
{
	for (TArray<TSharedPtr<ShortcutContainer>>::TIterator It(*ContainerList); It; ++It)
	{
		ShortcutContainerList.Add(*It);
	}
}

FText ASlAiPlayerState::GetShortcutInfoText() const
{
	return FText();
}
