// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class MEGAMANX_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

    UPROPERTY(EditAnywhere, Category = "Animation")
        class UPaperFlipbook* ItemFlipbook;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
        class USphereComponent* SphereCollision;

    UPROPERTY(Category = Character, VisibleAnywhere)
        class UPaperFlipbookComponent* ItemAnimation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
