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
	// 시간 멈춘거 풀기
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	// 마우스 커서 안보이게하기
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	this->RemoveFromParent();

}