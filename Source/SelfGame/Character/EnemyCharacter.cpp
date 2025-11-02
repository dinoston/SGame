#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "../UI/EnemyHealth.h" // UWBP_EnemyHealth
#include "../HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
    // 월드에 배치/스폰되면 자동으로 AI가 소유
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    WC_Health = CreateDefaultSubobject<UWidgetComponent>(TEXT("WC_Health"));
    WC_Health->SetupAttachment(GetMesh());                  // 또는 RootComponent
    WC_Health->SetWidgetSpace(EWidgetSpace::World);
    WC_Health->SetDrawAtDesiredSize(true);                  // 위젯 크기 그대로
    WC_Health->SetDrawSize(FVector2D(160.f, 16.f));         // 필요시 고정 크기
    WC_Health->SetPivot(FVector2D(0.5f, 0.0f));
    WC_Health->SetRelativeLocation(FVector(0, 0, 120.f));   // 머리 위 높이 조절
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (WC_Health)
    {
        if (UUserWidget* W = WC_Health->GetUserWidgetObject())
        {
            if (auto* UI = Cast<UWBP_EnemyHealth>(W))
            {
                // 캐릭터에 붙은 HealthComponent 찾기(ABaseCharacter에 이미 있음)
                if (UHealthComponent* HC = FindComponentByClass<UHealthComponent>())
                {
                    UI->BindToHealth(HC);
                }
            }
        }
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