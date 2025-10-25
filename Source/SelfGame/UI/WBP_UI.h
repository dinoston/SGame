// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_UI.generated.h"


class UProgressBar;
class UImage;
class UTextBlock;
class UHealthComponent;
class AWeapon;



/**
 * 화면 루트 UI: 체력바/조준점 등 표시
 * - InitFromPawn() 으로 HealthComponent에 구독
 * - 위젯 파괴 시 자동 해제
 */
UCLASS()
class SELFGAME_API UWBP_UI : public UUserWidget
{
    GENERATED_BODY()

public:
    /** HUD나 PC에서 위젯 생성 직후 한 번 호출 */
    UFUNCTION(BlueprintCallable)
    void InitFromPawn(APawn* Pawn);

protected:
    /** 디자이너 ProgressBar와 이름 동일해야 자동 바인딩됨 (예: PB_Health) */
    UPROPERTY(meta = (BindWidget))
    UProgressBar* PB_Health = nullptr;

    // ★ 탄약 텍스트 (디자이너에서 이름을 Text_Ammo로 만들어 두기)
    UPROPERTY(meta = (BindWidget)) 
    UTextBlock* Text_Ammo = nullptr;


    ///** (선택) 조준점 이미지가 있으면 BindWidgetOptional 로 잡아둠 */
    //UPROPERTY(meta = (BindWidgetOptional))
    //UImage* Image_Crosshair = nullptr;

    /** 현재 구독 중인 헬스 컴포넌트 */
    UPROPERTY()
    UHealthComponent* BoundHealth = nullptr;

    UPROPERTY() 
    AWeapon* BoundWeapon = nullptr;



    /** 델리게이트 콜백 */
    UFUNCTION()
    void OnHealthRatioChanged(float NewRatio);


    // ★ 무기 탄 이벤트 콜백
    UFUNCTION() 
    void OnAmmoChanged(int32 NewAmmo, int32 NewMax);

    UPROPERTY() 
    class ABaseCharacter* BoundCharacter = nullptr;

    UFUNCTION() 
    void OnCurrentWeaponChanged(class AWeapon* NewWeapon);


    /** 위젯 파괴 시 구독 해제 */
    virtual void NativeDestruct() override;

private:
    void BindToHealth(UHealthComponent* HC);
    void UnbindFromHealth();
    void ApplyRatioToUI(float Ratio);

    // ★ 무기 바인딩/해제
    void BindToWeapon(AWeapon* Wpn);
    void UnbindFromWeapon();
    void ApplyAmmoToUI(int32 Ammo, int32 MaxAmmo);

    void BindToCharacter(class ABaseCharacter* C);
    void UnbindFromCharacter();
};