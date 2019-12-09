// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include <Components/SphereComponent.h>
#include "PaperFlipbookComponent.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereCollision->SetSphereRadius(30.0f);
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereCollision->SetCollisionProfileName("OverlapAll");
    SphereCollision->SetGenerateOverlapEvents(true);
    SphereCollision->SetSimulatePhysics(false);
    SphereCollision->SetConstraintMode(EDOFMode::SixDOF);
    SetRootComponent(SphereCollision);

    ItemAnimation = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>("ItemAnimation");
    ItemAnimation->SetupAttachment(RootComponent);
    ItemAnimation->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ItemAnimation->SetGenerateOverlapEvents(false);

    Tags.Add("Pickup");


}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

