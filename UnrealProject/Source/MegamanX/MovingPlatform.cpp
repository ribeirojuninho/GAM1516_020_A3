// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PlatformBox = CreateDefaultSubobject<UBoxComponent>("PlatformBox");
    PlatformBox->SetBoxExtent(FVector(70.0f, 50.0f, 135.0f));
    PlatformBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PlatformBox->SetCollisionProfileName("BlockAll");
    PlatformBox->SetNotifyRigidBodyCollision(true);
    SetRootComponent(PlatformBox);

    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
    Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Sprite->SetCollisionProfileName("NoCollision");
    Sprite->SetSimulatePhysics(false);
    Sprite->SetGenerateOverlapEvents(false);
    Sprite->SetupAttachment(RootComponent);

    direction = 1;

    Tags.Add("MovingPlat");

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector NewLoc = GetActorLocation();

    if (NewLoc.Z > 600)
    {
        direction *= -1;
    }
    else if (NewLoc.Z < 0)
    {
        direction *= -1;
    }

    NewLoc.Z += direction * 2.0f;
    SetActorLocation(NewLoc);
}

