#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class AWeapon;

UCLASS()
class SELFGAME_API ABaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABaseCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    /** 체력 관리용 컴포넌트 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UHealthComponent* HealthComp;

    /** ───── Weapon ───── */
    /** 캐릭터 손의 무기 소켓 이름 (스켈레톤에 동일 이름 소켓을 만들어 두세요) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName WeaponSocketName = TEXT("Weapon_Socket"); // 예: "Hand_R" 추천


    /** 1번 무기/2번 무기의 클래스(BP 지정) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> PrimaryWeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> SecondaryWeaponClass;

    /** 실제 스폰된 무기 인스턴스들 */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    AWeapon* PrimaryWeapon = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    AWeapon* SecondaryWeapon = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon = nullptr;
    
    /** 입력 처리용: 발사/교체 */
    UFUNCTION() void FirePressed();
    UFUNCTION() void EquipPrimary();
    UFUNCTION() void EquipSecondary();


    //테스트로 0키로 50데미지//
    UFUNCTION()
    void TestTakeDamage();


private:
    /** 무기를 이 캐릭터 손 소켓에 부착 + 오너 지정 */
    void AttachWeapon(AWeapon* W);
};