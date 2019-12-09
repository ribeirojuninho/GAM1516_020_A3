// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasicCharacter.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is important to include
enum class EDirectionEnum : uint8
{
    DE_Right 	UMETA(DisplayName = "Right"),
    DE_Left 	UMETA(DisplayName = "Left")
};

UCLASS()
class MEGAMANX_API ABasicCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasicCharacter();

    UPROPERTY(EditDefaultsOnly)
        float FireRate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* FireSound;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* JumpSound;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* WalkSound;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* DashSound;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* LifeSound;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectile> ProjectileClass;

    FVector GetMuzzleLocation() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
        class UCapsuleComponent* CapsuleComponent;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Follow Camera", meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* FollowCameraComponent;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Spawn Components")
        class UArrowComponent* Muzzle;

    UFUNCTION()
        virtual	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
        virtual void MoveRight(float value);

    UFUNCTION()
        virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    virtual void Jump();
    virtual void Land();
    virtual void Dash();
    virtual void StopDash();
    virtual void SpawnProjectile();
    virtual void Fire();
    virtual void XBuster();
    virtual void ReleaseBuster();


    FTimerHandle FireTimer;

    //float TravelDirection;
    bool OnFloor = false;
    bool WasOnFloor = false;
    bool fire = false;
    bool dashing = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
        EDirectionEnum DirectionEnum;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
        class UPawnNoiseEmitterComponent* NoiseEmitterComponent;

    class AFloor* field;
    FVector CurrentVelocity;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UPaperSpriteComponent* PlayerSpriteComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class AXGameStateBase* XGameStateBase;

};
