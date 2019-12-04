// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ConstructorHelpers.h"
#include "PaperSprite.h"
//#include "MyBlueprintFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetSphereRadius(4.f);
    SphereComponent->BodyInstance.SetCollisionProfileName("BlockAll");
    SphereComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

    SetRootComponent(SphereComponent);

    ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperFlipbookComponent"));
    ProjectileSprite->SetupAttachment(RootComponent);
    ProjectileSprite->SetRelativeLocation(FVector(-3.0f, 0.0f, 0.0f));

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->UpdatedComponent = SphereComponent;
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 15000.0f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = false;
    ProjectileMovementComponent->bShouldBounce = true;

    InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != nullptr)
    {

    }
}

void AProjectile::FireDirection(FVector direction)
{
    ProjectileMovementComponent->Velocity = direction * ProjectileMovementComponent->InitialSpeed;
}