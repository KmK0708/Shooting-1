// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerFlight::APlayerFlight()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 박스 컴포넌트를 생성한다.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	// 생성된 충돌 박스 컴포넌트를 루트 컴포넌트로 설정한다.
	SetRootComponent(boxComp);
	
	// 박스 콜리전의 크기를 가로x세로x높이 모두 50센티미터로 설정한다.
	boxComp->SetBoxExtent(FVector(50.0f));

	// 박스 콜리전의 충돌 처리 프리셋을 "PlayerPreset"으로 설정한다.
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));

	// 메시 컴포넌트를 생성한다.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	// 메시 컴포넌트를 루트 컴포넌트의 하위 개체로 등록한다.
	meshComp->SetupAttachment(RootComponent);

	// 메시 컴포넌트의 static mesh 항목에 큐브 파일을 할당한다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	// 만일, 큐브 파일을 로드하는데 성공했다면...
	if (cubeMesh.Succeeded())
	{
		// 로드한 파일을 메시 컴포넌트의 static mesh 항목에 넣는다.
		meshComp->SetStaticMesh(cubeMesh.Object);
	}

}

void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* playerCon = Cast<APlayerController>(GetController());

	if (playerCon != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

		if (subsys != nullptr)
		{
			subsys->AddMappingContext(imc_myMapping, 0);
		}
	}

	// 현재 색상 값을 저장한다.
	UMaterialInterface* iMat = meshComp->GetMaterial(0);
	FHashedMaterialParameterInfo param = FHashedMaterialParameterInfo(TEXT("my color"));

	//Material Interface 에서 벡터 파라미터 값을 initColor 변수에 저장한다.
	iMat->GetVectorParameterValue(param, initColor);

	UE_LOG(LogTemp, Warning, TEXT("R: %f, G: %f, B: %f"), initColor.R, initColor.G, initColor.B);
	// Material Interface 를 이용해서 Material Instance Dynamic 개체를 만든다.
	dynamicMat = UMaterialInstanceDynamic::Create(iMat, this);

	// 생성한 다이나믹 매터리얼을 메시에 설정한다.
	if (dynamicMat != nullptr)
	{
		meshComp->SetMaterial(0, dynamicMat);
	}
	
}

void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 사용자가 입력한 방향대로 이동하고 싶다.
	// 벡터의 정규화
	direction.Normalize();

	// P = P0 + vt
	FVector dir = GetActorLocation() + direction * moveSpeed * DeltaTime;
	SetActorLocation(dir, true);

}

void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// 기존의 UInputComponent*를 UEnhancedInputComponent*로 변환한다.
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// 변환된 enhancedInputComponent의 멤버 함수 BindAction을 이용해서 입력을 바인딩한다.
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);

	//// Horizontal Axis 입력에 함수를 연결한다.
	//PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);
	//
	//// Vertical Axis 입력에 함수를 연결한다.
	//PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical);

	//// Fire Action 입력에 함수를 연결한다.
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

// 맞았을시 색변환 함수
void APlayerFlight::ReservationHitColor(float time)
{
	// 1. 색상을 red 색으로 변경한다
	dynamicMat->SetVectorParameterValue(TEXT("my color"), (FVector4)FLinearColor::Red);
	// 2. 원래 색상으로 되돌리는 함수를 바인딩한 타이머를 예약한다.

	GetWorld()->GetTimerManager().SetTimer(colorTimer, this, &APlayerFlight::ChangeOriginColor, time, false);
}



void APlayerFlight::ChangeOriginColor()
{
	// 색을 알아야하니까 헤더에 프라이빗 변수 하나 선언
	// begin에서 색상값을 저장하며 시작
	dynamicMat->SetVectorParameterValue(TEXT("my color"), (FVector4)initColor);
	//my_mat->SetVectorParameterValue(TEXT("Color"), FLinearColor(255, 0, 0, 255)); 위와 동일한 코드
}
// 좌우 입력이 있을 때 실행될 함수
//void APlayerFlight::Horizontal(float value)
//{
//	h = value;
//	//UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
//	direction.Y = h;
//}

void APlayerFlight::Horizontal(const FInputActionValue& value)
{
	h = value.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
	direction.Y = h;
}

// 상하 입력이 있을 때 실행될 함수
//void APlayerFlight::Vertical(float value)
//{
//	v = value;
//	//UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
//	direction.Z = v;
//}

void APlayerFlight::Vertical(const FInputActionValue& value)
{
	v = value.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
	direction.Z = v;
}

// 마우스 왼쪽 버튼을 눌렀을 때 실행될 함수
void APlayerFlight::FireBullet()
{
	// 총알을 생성한다.
	// 총알 블루프린트 변수
	FVector spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
	FRotator spawnRotation = FRotator(90.0f, 0, 0);
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnPosition, spawnRotation, param);

	//총알 발사 효과음을 생성.
	UGameplayStatics::PlaySound2D(this, fireSound);
}


