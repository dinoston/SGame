#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Enum/WeaponTypes.h"

#include "BaseCharacter.generated.h"



class UHealthComponent;
class AWeapon;
class USpringArmComponent;
class UCameraComponent;


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

    //--------------Components-------------//
    /** 체력  */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UHealthComponent* HealthComp;


    //카메라, 스프링암//
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> Camera;

    //걷기, 뛰기 //

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 400.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 700.f;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Running = nullptr;  // 에디터에서 IA_Running 지정

    void OnRunPressed(const struct FInputActionValue& Value);
    void OnRunReleased(const struct FInputActionValue& Value);
    
    

   
    /** ───── Weapon ───── */
    /** 캐릭터 손의 무기 소켓 이름 (스켈레톤에 동일 이름 소켓을 만들어 두세요) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName WeaponSocketName = TEXT("WeaponSocket"); // 예: "Hand_R" 추천


    // ───── Weapon 상태(enum) ─────
    /** 현재 장착된 무기 타입 (AnimBP에서 이 값 읽어서 포즈 전환) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponType CurrentWeaponType = EWeaponType::None;

    /** 외부(픽업 등)에서 타입으로 장착 */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipWeaponType(EWeaponType NewType);

    /** 전부 해제(맨손) */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void UnEquipAll();



    


    /** 1번 무기/2번 무기의 클래스(BP 지정) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> PistolWeaponClass; // Pistol

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> RifleWeaponClass; // Rifle

    /** 실제 스폰된 무기 인스턴스들 */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    AWeapon* PistolWeapon = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    AWeapon* RifleWeapon = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon = nullptr;
    
    /** 컨트롤러에서 호출할 행동들 */    
    UFUNCTION(BlueprintCallable) 
    void FirePressed();
    UFUNCTION(BlueprintCallable) 
    void EquipPistol(); // 내부적으로 EquipWeaponType(EWeaponType::Pistol)
    UFUNCTION(BlueprintCallable) 
    void EquipRifle(); // 내부적으로 EquipWeaponType(EWeaponType::Rifle)


    //테스트로 0키로 50데미지//
    UFUNCTION(BlueprintCallable)
    void TestTakeDamage();


private:
    /** 무기를 이 캐릭터 손 소켓에 부착 + 오너 지정 */
    void AttachWeapon(AWeapon* W);

};