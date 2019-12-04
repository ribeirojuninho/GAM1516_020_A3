// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasicCharacter.generated.h"

UCLASS()
class MEGAMANX_API ABasicCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasicCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
        class UCapsuleComponent* CapsuleComponent;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Follow Camera", meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* FollowCameraComponent;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* SpringArmComponent;
    
    UFUNCTION()
        virtual	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
        virtual void MoveRight(float value);

    virtual void Jump();
    virtual void Land();

    float TravelDirection;
    bool OnFloor = false;
    bool WasOnFloor = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UPaperSpriteComponent* PlayerSpriteComponent;

    float JumpScalar = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        float MaxVelX = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        float ForceScalarX = 500000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        float AirborneScalarX = 15000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        float ForceScalarZ = 800000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        float JumpPowerZ = 40.0f;
};
