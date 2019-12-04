// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MegamanX.h"
#include "CoreMinimal.h"
#include "BasicCharacter.h"
#include "CharacterAnimation.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMANX_API ACharacterAnimation : public ABasicCharacter
{
    GENERATED_BODY()

public:
        ACharacterAnimation();

        UPROPERTY(EditAnywhere, Category = "Animation")
            class UPaperFlipbook* IdleFlipbook;

        UPROPERTY(EditAnywhere, Category = "Animation")
            class UPaperFlipbook* WalkingFlipbook;

        UPROPERTY(EditAnywhere, Category = "Animation")
            class UPaperFlipbook* JumpFlipbook;

        UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
            EAnimState AnimState = EAnimState::Idle;

        virtual void Tick(float DeltaTime) override;

protected:

    virtual void MoveRight(float value) override;

    virtual void BeginPlay() override;

    UPROPERTY(Category = Character, VisibleAnywhere)
        class UPaperFlipbookComponent* MegamanXAnimation;

    virtual void UpdateAnimation();

    UFUNCTION(BlueprintCallable, Category = "ChangeState")
        void SetState(EAnimState NewState) { AnimState = NewState; }

    virtual void Jump() override;
    virtual void Land() override;

    virtual	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
private:

};
