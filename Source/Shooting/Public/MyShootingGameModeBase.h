// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyShootingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMyShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class UMainWidget> mainWidget;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentScore() { return currentScore;}

	virtual void BeginPlay() override;

	// 인라인 함수 헤더에서 간단하게 선언하기 
	// 단 , 1줄까지만.

	void AddScore(int32 count);

private:
	int32 currentScore = 0;


	
};
