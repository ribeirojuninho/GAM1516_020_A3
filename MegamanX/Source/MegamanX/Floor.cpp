// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "ConstructorHelpers.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

   // ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSpriteRef(TEXT(""));
    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
   /* if (PaperSpriteRef.Succeeded())
    {
        Sprite->SetSprite(PaperSpriteRef.Object);
    }*/
    SetRootComponent(Sprite);
    Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Sprite->SetCollisionProfileName("NoCollision");
    Sprite->SetSimulatePhysics(false);
    Sprite->SetGenerateOverlapEvents(false);

    FloorBox = CreateDefaultSubobject<UBoxComponent>("FloorBox");
    FloorBox->SetupAttachment(RootComponent);
    FloorBox->SetBoxExtent(FVector(350.0f, 50.0f, 20.0f));
    FloorBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    FloorBox->SetCollisionProfileName("BlockAll");
    FloorBox->SetNotifyRigidBodyCollision(true);
    FloorBox->SetRelativeLocation(FVector(18.0f, 0.0f, -45.0f));

    LeftBox = CreateDefaultSubobject<UBoxComponent>("LeftBox");
    LeftBox->SetupAttachment(RootComponent);
    LeftBox->SetBoxExtent(FVector(10.0f, 50.0f, 70.0f));
    LeftBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    LeftBox->SetCollisionProfileName("BlockAll");
    LeftBox->SetNotifyRigidBodyCollision(true);
    LeftBox->SetRelativeLocation(FVector(-300.0f, 0.0f, 30.0f));
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

