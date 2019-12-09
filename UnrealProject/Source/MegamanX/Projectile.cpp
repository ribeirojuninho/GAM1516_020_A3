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
#include "EngineUtils.h"
#include "Floor.h"
#include <Components/ArrowComponent.h>

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetSphereRadius(10.0f);
    SphereComponent->BodyInstance.SetCollisionProfileName("BlockAll");
    SphereComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

    SetRootComponent(SphereComponent);

    ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperFlipbookComponent"));
    ProjectileSprite->SetupAttachment(RootComponent);
    ProjectileSprite->SetRelativeLocation(FVector(-3.0f, 0.0f, 0.0f));

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovement->UpdatedComponent = SphereComponent;
    ProjectileMovement->InitialSpeed = 1200.0f;
    ProjectileMovement->MaxSpeed = 15000.0f;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = true;

    InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

    for (TActorIterator<AFloor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr)
        {
            field = *ActorItr;
            break;
        }

    }
	
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
        if (!OtherActor->GetOwner())
        {
            if (!OtherActor->ActorHasTag("MovingPlat") && !OtherComp->ComponentHasTag("SecondFloor") && !OtherComp->ComponentHasTag("Goal") && !OtherComp->ComponentHasTag("InvisibleWall") && !OtherComp->ComponentHasTag("Floor"))
            {
                UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
                if (OtherActor->ActorHasTag("Player"))
                {
                    FVector checkpointloc = field->Checkpoint->GetComponentLocation();
                    OtherActor->SetActorLocation(checkpointloc);
                }
                else {
                    OtherActor->Destroy();
                    this->Destroy();
                }
                
            }
            this->Destroy();
        }
        
 
    }
}

void AProjectile::FireDirection(FVector direction)
{
    ProjectileMovement->Velocity = direction * ProjectileMovement->InitialSpeed;
}