// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerFlight::APlayerFlight()
{
	PrimaryActorTick.bCanEverTick = true;

	// �浹 �ڽ� ������Ʈ�� �����Ѵ�.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	// ������ �浹 �ڽ� ������Ʈ�� ��Ʈ ������Ʈ�� �����Ѵ�.
	SetRootComponent(boxComp);
	
	// �ڽ� �ݸ����� ũ�⸦ ����x����x���� ��� 50��Ƽ���ͷ� �����Ѵ�.
	boxComp->SetBoxExtent(FVector(50.0f));

	// �޽� ������Ʈ�� �����Ѵ�.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	// �޽� ������Ʈ�� ��Ʈ ������Ʈ�� ���� ��ü�� ����Ѵ�.
	meshComp->SetupAttachment(RootComponent);

	// �޽� ������Ʈ�� static mesh �׸� ť�� ������ �Ҵ��Ѵ�.
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	// ����, ť�� ������ �ε��ϴµ� �����ߴٸ�...
	if (cubeMesh.Succeeded())
	{
		// �ε��� ������ �޽� ������Ʈ�� static mesh �׸� �ִ´�.
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
}

void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ����ڰ� �Է��� ������ �̵��ϰ� �ʹ�.
	// ������ ����ȭ
	direction.Normalize();

	// P = P0 + vt
	FVector dir = GetActorLocation() + direction * moveSpeed * DeltaTime;
	SetActorLocation(dir);

}

void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerFlight::Horizontal);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerFlight::Vertical);
	enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerFlight::FireBullet);

	//// Horizontal Axis �Է¿� �Լ��� �����Ѵ�.
	//PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::Horizontal);
	//
	//// Vertical Axis �Է¿� �Լ��� �����Ѵ�.
	//PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::Vertical);

	//// Fire Action �Է¿� �Լ��� �����Ѵ�.
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerFlight::FireBullet);
}

// �¿� �Է��� ���� �� ����� �Լ�
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

// ���� �Է��� ���� �� ����� �Լ�
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

// ���콺 ���� ��ư�� ������ �� ����� �Լ�
void APlayerFlight::FireBullet()
{
	// �Ѿ��� �����Ѵ�.
	// �Ѿ� �������Ʈ ����
	FVector spawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
	FRotator spawnRotation = FRotator(90.0f, 0, 0);
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnPosition, spawnRotation, param);
}


