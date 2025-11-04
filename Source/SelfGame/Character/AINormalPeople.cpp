// Fill out your copyright notice in the Description page of Project Settings.

#include "AINormalPeople.h"
#include "../HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"   
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"  

// Sets default values
AAINormalPeople::AAINormalPeople()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	// 이동 파라미터(원하면 조정)
	GetCharacterMovement()->MaxWalkSpeed = 250.f;

}

// Called when the game starts or when spawned
void AAINormalPeople::BeginPlay()
{
	Super::BeginPlay();
	
	// UE 표준 데미지 파이프라인 수신 → 우리 HealthComp로 전달
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnAnyDamage);

	// 죽음 이벤트(HealthComponent에 이미 OnDeath 있다면 거기 바인딩)
	if (HealthComp)
		HealthComp->OnDeath.AddDynamic(this, &ThisClass::OnDead);

}

void AAINormalPeople::OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType*, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComp && Damage > 0.f)
		HealthComp->TakeDamage(Damage); // 네가 만든 TakeDamage(float) 재사용
}

void AAINormalPeople::OnDead()
{
	// 1) 데스 몽타주 재생 or 래그돌
	// 2) AI 정지 / 콜리전 끄기 / 일정 시간 뒤 Destroy 등
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Destroy를 원하면 타이머로 지우기
	// SetLifeSpan(5.f);
}

// Called every frame
void AAINormalPeople::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAINormalPeople::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

