// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootingGameModeBase.h"
#include "MainWidget.h"
#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"

void AMyShootingGameModeBase::BeginPlay()
{
	// ���� �������Ʈ�� �����Ѵ�.
	main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	
	if (main_UI != nullptr)
	{
		// ������ ������ �� ��Ʈ�� �׸���.
		main_UI->AddToViewport();


		// �ְ� ������ ���Ͽ��� �о�´�.
		FString scoreText;
		bool isLoadSuccess = FFileHelper::LoadFileToString(scoreText, *filePath);
// 		if (isLoadSuccess)
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Success"));
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Failed"));
// 		}

		// UE_LOG(LogTemp, Warning, TEXT("%s"), isLoadSuccess);

		// �о�� ���� bestScore ������ �ִ´�.
		bestScore = FCString::Atoi(*scoreText);

		// ���� ������ ������ curScore �ؽ�Ʈ ��Ͽ� �ݿ��Ѵ�.
		main_UI->PrintCurrentScore();

	}
}

void AMyShootingGameModeBase::AddScore(int32 count)
{
	currentScore += count;

	// ����, ������ ���� ������ �ְ��������� ũ�ٸ�,
	if (currentScore > bestScore)
	{
		// �ְ������� ���� ������ (����)�ִ´�.
		bestScore = currentScore;

		// �ְ� ������ ���Ϸ� �����Ѵ�. (����ٺ��� �ؽ�Ʈ����..)
		
		bool isSaved =  FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);

		//UE_LOG(LogTemp, Warning, TEXT("%s"), filePath ? TEXT("True") : TEXT("False"));
	}
	

	// ���� ������ ������ curScore �ؽ�Ʈ ��Ͽ� �ݿ��Ѵ�.
	if (main_UI != nullptr)
	{
		main_UI->PrintCurrentScore();
	}
}

void AMyShootingGameModeBase::ShowMenu()
{
	menu_UI = CreateWidget<UMenuWidget>(GetWorld(),menuWidget);

	if (menu_UI != nullptr)
	{
		menu_UI->AddToViewport();
	}

	// ������ �Ͻ� ���� ���·� �����.
	// 1. ���� ������ �ð� �帧�� 0���� �����.
	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);

	// 2. �Ͻ����� �Լ��� ȣ���Ѵ�.
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// ���콺 Ŀ���� ȭ�鿡 ���̰� �Ѵ�.
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	// �ٽ� �����ҽ�,
	// 1. �ð��� �帣�� �Ѵ�.

	// 2. menu widget����

	// 3. ���콺 Ŀ�� �Ⱥ��̰� �ϱ�
}
