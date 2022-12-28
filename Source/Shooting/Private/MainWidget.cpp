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
		// �Ű������� 1������
		// ue log�� ������ ������ 2~3���� ����

		// ���� ����(����) -> FText (���ڿ�) ���·� ��ȯ�ؼ�
		FText ScoreText = FText::AsNumber(myGM->GetCurrentScore());
		// curScore �ؽ�Ʈ ����� ������ �����Ѵ�.
		curScore->SetText(ScoreText);
	}

	
}
