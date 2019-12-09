// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIReploid.generated.h"

UCLASS()
class MEGAMANX_API AAIReploid : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIReploid();

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AiSprite")
        class UPaperSpriteComponent* AiSprite;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collision")
        class UCapsuleComponent* AiCapsule;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Spawn Components")
        class UArrowComponent* Muzzle;

    virtual void PostInitializeComponents() override;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly)
        float FireRate = 2.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
        class UPawnSensingComponent* PawnSenseComp;

    UFUNCTION()
        void PawnSeen(APawn* SeenPawn);

    UFUNCTION()
        void NoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

    FTimerHandle FireTimer;

    virtual void SpawnProjectile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void MoveRight(float AxisValue);

    FVector CurrentVelocity;

    AActor* TargetActor;
    FVector HeardLocation;
};
