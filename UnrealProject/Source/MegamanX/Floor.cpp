// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "ConstructorHelpers.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include <Components/ArrowComponent.h>
#include "CharacterAnimation.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

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
    FloorBox->SetBoxExtent(FVector(380.0f, 50.0f, 20.0f));
    FloorBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    FloorBox->SetCollisionProfileName("BlockAll");
    FloorBox->SetNotifyRigidBodyCollision(true);
    FloorBox->SetRelativeLocation(FVector(18.0f, 0.0f, -45.0f));

    SecondFloorBox = CreateDefaultSubobject<UBoxComponent>("SecondFloorBox");
    SecondFloorBox->SetupAttachment(RootComponent);
    SecondFloorBox->SetBoxExtent(FVector(250.0f, 50.0f, 20.0f));
    SecondFloorBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SecondFloorBox->SetCollisionProfileName("BlockAll");
    SecondFloorBox->SetNotifyRigidBodyCollision(true);
    SecondFloorBox->SetRelativeLocation(FVector(420.0f, 0.0f, -5.0f));

    LeftBox = CreateDefaultSubobject<UBoxComponent>("LeftBox");
    LeftBox->SetupAttachment(RootComponent);
    LeftBox->SetBoxExtent(FVector(10.0f, 50.0f, 70.0f));
    LeftBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    LeftBox->SetCollisionProfileName("BlockAll");
    LeftBox->SetNotifyRigidBodyCollision(true);
    LeftBox->SetRelativeLocation(FVector(-360.0f, 0.0f, 30.0f));

    RightBox = CreateDefaultSubobject<UBoxComponent>("RightBox");
    RightBox->SetupAttachment(RootComponent);
    RightBox->SetBoxExtent(FVector(10.0f, 50.0f, 70.0f));
    RightBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RightBox->SetCollisionProfileName("OverlapAll");
    RightBox->SetRelativeLocation(FVector(644.0, 0.0f, 66.0f));
    

    OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
    OverlapBox->SetupAttachment(RootComponent);
    OverlapBox->SetBoxExtent(FVector(50.0f, 50.0f, 20.0f));
    OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapBox->SetCollisionProfileName("OverlapAll");
    OverlapBox->SetRelativeLocation(FVector(440.0f, 0.0f, -87.0f));
    

    CheckpointBox = CreateDefaultSubobject<UBoxComponent>("CheckpointBox");
    CheckpointBox->SetupAttachment(RootComponent);
    CheckpointBox->SetBoxExtent(FVector(50.0f, 50.0f, 20.0f));
    CheckpointBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CheckpointBox->SetCollisionProfileName("OverlapAll");
    CheckpointBox->SetRelativeLocation(FVector(440.0f, 0.0f, -87.0f));
    

    Checkpoint = CreateDefaultSubobject<UArrowComponent>("Checkpoint");
    Checkpoint->SetupAttachment(RootComponent);

    CheckpointBox->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnOverlapBegin);
    OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnOverlapBegin2);
    RightBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFloor::OnOverlapBegin3);
    //RightBox->OnComponentEndOverlap.AddUniqueDynamic(this, &AFloor::OverlapEnds);

    FloorBox->ComponentTags.Add("Floor");
    SecondFloorBox->ComponentTags.Add("SecondFloor");
    LeftBox->ComponentTags.Add("InvisibleWall");
    RightBox->ComponentTags.Add("Goal");
    OverlapBox->ComponentTags.Add("OverlapFloor");
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();

    for (TActorIterator<ACharacterAnimation> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr)
        {
            player = *ActorItr;
            break;
        }

    }
    
}

void AFloor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor ==  player)
    {
        Checkpoint->SetWorldLocation(FVector(665.0, 20.0f, 800.0f));
    }
}


void AFloor::OnOverlapBegin2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == player)
    {
        FVector checkpointloc = Checkpoint->GetComponentLocation();
        player->SetActorLocation(checkpointloc);
    }
}

void AFloor::OnOverlapBegin3(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
    if (OtherActor == player && NextLevel != "")
    {
        
        UGameplayStatics::OpenLevel(this, NextLevel, true);
        
    }
}


// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

