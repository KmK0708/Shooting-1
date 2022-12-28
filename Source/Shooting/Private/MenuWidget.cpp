// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuWidget::NativeConstruct()
{
	btn_Resume->OnClicked.AddDynamic(this, &UMenuWidget::ResumeGame);
}

void UMenuWidget::ResumeGame()
{
	// �ð� ����� Ǯ��
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	// ���콺 Ŀ�� �Ⱥ��̰��ϱ�
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	this->RemoveFromParent();

}