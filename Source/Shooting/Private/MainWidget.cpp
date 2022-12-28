// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "MyShootingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"


void UMainWidget::PrintCurrentScore()
{
	AMyShootingGameModeBase* myGM = Cast<AMyShootingGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (myGM != nullptr)
	{
		// 매개변수는 1개고정
		// ue log는 가변형 변수라 2~3개씩 가능

		// 현재 점수(정수) -> FText (문자열) 형태로 변환해서
		FText ScoreText = FText::AsNumber(myGM->GetCurrentScore());
		// curScore 텍스트 블록의 값으로 설정한다.
		curScore->SetText(ScoreText);
	}

	
}
