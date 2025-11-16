#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"

#include "../Player/EnemyAIController.h"
#include "../UI/EnemyHealth.h" // UWBP_EnemyHealth
#include "../HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
    // 월드에 배치/스폰되면 자동으로 AI가 소유
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AEnemyAIController::StaticClass();




    WC_Health = CreateDefaultSubobject<UWidgetComponent>(TEXT("WC_Health"));
    WC_Health->SetupAttachment(GetMesh());                  // 또는 RootComponent
    WC_Health->SetWidgetSpace(EWidgetSpace::World);
    WC_Health->SetDrawAtDesiredSize(true);                  // 위젯 크기 그대로
    WC_Health->SetDrawSize(FVector2D(160.f, 16.f));         // 필요시 고정 크기
    WC_Health->SetPivot(FVector2D(0.5f, 0.0f));
    WC_Health->SetRelativeLocation(FVector(0, 0, 120.f));   // 머리 위 높이 조절

    // ★ 위젯 클래스를 코드에서 고정(에디터에서 안넣어도 안전)
    WC_Health->SetWidgetClass(UWBP_EnemyHealth::StaticClass());
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // ★ 위젯 인스턴스 보장
    if (WC_Health && !WC_Health->GetUserWidgetObject())
    {
        WC_Health->InitWidget(); // WidgetClass 기반으로 즉시 생성
    }

    if (UHealthComponent* HC = FindComponentByClass<UHealthComponent>())
    {
        // 위젯 꺼내서 바인딩D
        if (auto* UI = Cast<UWBP_EnemyHealth>(WC_Health ? WC_Health->GetUserWidgetObject() : nullptr))
        {
            UI->BindToHealth(HC);               // 체력 변화 구독
            // 초기값은 BindToHealth 안에서 바로 반영하도록 이미 구현했지
        }

        // 죽으면 머리 위 체력바 감추기 (보기 깔끔)
        HC->OnDeath.AddDynamic(this, &ThisClass::OnEnemyDeath);
    }

    if (!IsPlayerControlled())
    {
        if (Camera) { Camera->DestroyComponent(); }
        if (SpringArm) { SpringArm->DestroyComponent(); }
    }
}

FVector AEnemyCharacter::GetPawnViewLocation() const
{
    static const FName HeadSocket(TEXT("head")); // 프로젝트 소켓명에 맞게 수정
    if (const USkeletalMeshComponent* M = GetMesh())
        if (M->DoesSocketExist(HeadSocket))
            return M->GetSocketLocation(HeadSocket);
    return Super::GetPawnViewLocation();
}

void AEnemyCharacter::OnEnemyDeath()
{
    if (WC_Health) WC_Health->SetVisibility(false);
    // 필요하면 여기서 Ragdoll/Destroy 타이머 등도 처리
}