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


    /** ü�� ������ ������Ʈ */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UHealthComponent* HealthComp;

    /** ���������� Weapon ���������� */
    /** ĳ���� ���� ���� ���� �̸� (���̷��濡 ���� �̸� ������ ����� �μ���) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName WeaponSocketName = TEXT("Weapon_Socket"); // ��: "Hand_R" ��õ


    /** 1�� ����/2�� ������ Ŭ����(BP ����) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> PrimaryWeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> SecondaryWeaponClass;

    /** ���� ������ ���� �ν��Ͻ��� */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    AWeapon* PrimaryWeapon = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    AWeapon* SecondaryWeapon = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon = nullptr;
    
    /** �Է� ó����: �߻�/��ü */
    UFUNCTION() void FirePressed();
    UFUNCTION() void EquipPrimary();
    UFUNCTION() void EquipSecondary();


    //�׽�Ʈ�� 0Ű�� 50������//
    UFUNCTION()
    void TestTakeDamage();


private:
    /** ���⸦ �� ĳ���� �� ���Ͽ� ���� + ���� ���� */
    void AttachWeapon(AWeapon* W);
};