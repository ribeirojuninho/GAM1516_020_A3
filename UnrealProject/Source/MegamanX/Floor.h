// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

UCLASS()
class MEGAMANX_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Sprite")
        class UPaperSpriteComponent* Sprite;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collision Component")
        class UBoxComponent* FloorBox;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collision Component")
        class UBoxComponent* LeftBox;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collision Component")
        class UBoxComponent* OverlapBox;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collision Component")
        class UBoxComponent* RightBox;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collision Component")
        class UBoxComponent* SecondFloorBox;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collision Component")
        class UBoxComponent* CheckpointBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Spawn Components")
        class UArrowComponent* Checkpoint;

    UPROPERTY(EditAnywhere)
        FName NextLevel;
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
        void OnOverlapBegin2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
        void OnOverlapBegin3(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

 public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

   
    class ACharacterAnimation* player;
};
