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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
