

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"
#include "Engine/Engine.h"


constexpr ECollisionChannel BULLET_CH = ECC_GameTraceChannel1; // Project Settings → Collision에서 만든 TraceChannel(Bullet)


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);              // ← 루트의 자식으로 부착
	// Mesh->SetRelativeRotation(FRotator(0,90,0)); // 기본 회전 미리 줄 수도 있음
}



void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerChar = Cast<ACharacter>(GetOwner());
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// ===== 내부 유틸 =====

bool AWeapon::CanFireNow() const
{
	// FireRate: 초당 발사수 → 최소 간격
	const double MinInterval = (FireRate > 0.f) ? (1.f / FireRate) : 0.0;
	const double Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;

	const bool CooldownOK = (MinInterval <= 0.0) || (Now - LastFireTime >= MinInterval);
	const bool AmmoOK = (Ammo < 0) || (Ammo > 0);
	return CooldownOK && AmmoOK;
}

int32 AWeapon::AddAmmo(int32 Amount)
{
    // 무한 탄(-1)은 보급 의미 없음
    if (Ammo < 0) return 0;

    const int32 Old = Ammo;
    Ammo = FMath::Clamp(Ammo + Amount, 0, MaxAmmo);

    return Ammo - Old;
}

FTransform AWeapon::GetMuzzleTransform() const
{
	if (Mesh && Mesh->DoesSocketExist(MuzzleSocketName))
	{
		return Mesh->GetSocketTransform(MuzzleSocketName, RTS_World);
	}

	// 소켓이 없으면 메시 기준 폴백
	const FRotator Rot = Mesh ? Mesh->GetComponentRotation() : FRotator::ZeroRotator;
	const FVector  Loc = Mesh ? Mesh->GetComponentLocation() : FVector::ZeroVector;
	return FTransform(Rot, Loc);
}

void AWeapon::PlayMuzzleFX(const FTransform& MuzzleXf) const
{
    // 1) 먼저 널 체크 후 바로 리턴
    if (!MuzzleFX)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] MuzzleFX not set"), *GetName());
        return;
    }

    // 2) 소켓이 있으면 붙여서, 없으면 월드 좌표로
    if (Mesh && Mesh->DoesSocketExist(MuzzleSocketName))
    {
        UGameplayStatics::SpawnEmitterAttached(
            MuzzleFX,
            Mesh,
            MuzzleSocketName,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true // bAutoDestroy
        );
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Muzzle socket '%s' not found – spawning at world loc"),
            *GetName(), *MuzzleSocketName.ToString());

        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            MuzzleFX,
            MuzzleXf
        );
    }

    // 사운드
    if (FireSound)
    {
        if (Mesh && Mesh->DoesSocketExist(MuzzleSocketName))
            UGameplayStatics::SpawnSoundAttached(FireSound, Mesh, MuzzleSocketName);
        else
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleXf.GetLocation());
    }
}

void AWeapon::PlayImpactFX(const FHitResult& Hit) const
{
    if (ImpactFX)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ImpactFX,
            Hit.ImpactPoint,
            Hit.ImpactNormal.Rotation()
        );
    }

    if (ImpactSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.ImpactPoint);
    }
}

void AWeapon::Fire()
{
    if (!GetWorld() || !CanFireNow())
        return;

    // ★ 탄약 0이면 딱-딱 사운드만 재생하고 종료
    if (Ammo == 0)
    {
        // 머즐 소켓에서 나게 하거나, 위치 없으면 액터 위치에서
        if (EmptySound)
        {
            if (Mesh && Mesh->DoesSocketExist(MuzzleSocketName))
                UGameplayStatics::SpawnSoundAttached(EmptySound, Mesh, MuzzleSocketName);
            else
                UGameplayStatics::PlaySoundAtLocation(this, EmptySound,
                    Mesh ? Mesh->GetComponentLocation() : GetActorLocation());
        }
        return;
    }

    // ① 0발이면 발사 차단 + 안내
    if (Ammo == 0) {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No ammo!"));
        }
        return;
    }

    const FTransform MuzzleXf = GetMuzzleTransform();
    const FVector Start = MuzzleXf.GetLocation();
    const FVector Dir = MuzzleXf.GetRotation().Vector();


    // 탄약 소모(무한탄 예외: Ammo<0 는 소모 안 함)
    if (Ammo > 0) {
        Ammo = FMath::Max(Ammo - 1, 0);  // 음수 방지
    }

    // ② 남은 탄 화면 표기
    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(
            -1, 1.0f, FColor::Green,
            FString::Printf(TEXT("Ammo: %d / %d"), Ammo, MaxAmmo));
    }

    // 머즐 FX/사운드
    PlayMuzzleFX(MuzzleXf);

    // 투사체 모드
    if (ProjectileClass)
    {
        FActorSpawnParameters Params;
        Params.Owner = this;
        Params.Instigator = OwnerChar; // 선택: 데미지 instigator 연동
        Params.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleXf, Params);
    }
    // 히트스캔 모드
    else
    {
        const FVector End = Start + Dir * Range;

        FHitResult Hit;

        // ▼ 디버그/머티리얼 옵션 추가
        FCollisionQueryParams QParams(SCENE_QUERY_STAT(WeaponTrace), /*bTraceComplex*/ true, this);
        QParams.bReturnPhysicalMaterial = true;      // 표면별 FX 쓸 때 유용
        QParams.TraceTag = TEXT("WeaponTrace");      // 디버그 태그

        if (OwnerChar) QParams.AddIgnoredActor(OwnerChar);
        QParams.AddIgnoredActor(this);

        // ▼ 커스텀 트레이스 채널 사용 (Bullet)
        if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, BULLET_CH, QParams))
        {
            // (선택) 디버그 라인
            DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Red, false, 1.0f, 0, 1.5f);

            if (AActor* Target = Hit.GetActor())
            {
                TSubclassOf<UDamageType> DamageTypeClass = DamageType;
                if (!DamageTypeClass) DamageTypeClass = UDamageType::StaticClass();

                UGameplayStatics::ApplyPointDamage(
                    Target, Damage, Dir, Hit,
                    OwnerChar ? OwnerChar->GetController() : nullptr,
                    this, DamageTypeClass);
            }

            // 임팩트 FX/사운드
            PlayImpactFX(Hit);
        }
        else
        {
            // (선택) 미스샷 디버그 라인
            DrawDebugLine(GetWorld(), Start, End, FColor::Silver, false, 1.0f, 0, 1.5f);
        }
    }

    LastFireTime = GetWorld()->GetTimeSeconds();
}








