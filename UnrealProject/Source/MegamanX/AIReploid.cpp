// Fill out your copyright notice in the Description page of Project Settings.


#include "AIReploid.h"
#include "Components/CapsuleComponent.h"
#include "XBlueprintFunctionLibrary.h"
#include "Classes/PaperSpriteComponent.h"
#include "Perception/PawnSensingComponent.h"
#include <Components/ArrowComponent.h>
#include "Projectile.h"

// Sets default values
AAIReploid::AAIReploid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    AiCapsule = CreateDefaultSubobject<UCapsuleComponent>("AiCapsule");
    AiCapsule->InitCapsuleSize(55.0f, 75.0f);
    AiCapsule->SetCollisionProfileName("BlockAll");
    AiCapsule->SetShouldUpdatePhysicsVolume(true);
    AiCapsule->SetCanEverAffectNavigation(false);
    AiCapsule->SetSimulatePhysics(true);
    AiCapsule->SetNotifyRigidBodyCollision(true);
    UXBlueprintFunctionLibrary::LockPhysicsTo2DAxis(AiCapsule);
    SetRootComponent(AiCapsule);

    AiSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
    AiSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AiSprite->SetCollisionProfileName("NoCollision");
    AiSprite->SetSimulatePhysics(false);
    AiSprite->SetEnableGravity(false);
    AiSprite->SetGenerateOverlapEvents(false);
    AiSprite->SetupAttachment(RootComponent);

    PawnSenseComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSenseComp");
    PawnSenseComp->OnSeePawn.AddDynamic(this, &AAIReploid::PawnSeen);
    PawnSenseComp->OnHearNoise.AddDynamic(this, &AAIReploid::NoiseHeard);

    Muzzle = CreateDefaultSubobject<UArrowComponent>("Muzzle");
    Muzzle->SetupAttachment(RootComponent);

    Tags.Add("AiPlayer");
}

void AAIReploid::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AAIReploid::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAIReploid::PawnSeen(APawn* SeenPawn)
{
    if (SeenPawn == nullptr)
    {
        return;
    }

    TargetActor = SeenPawn;

    //only shoot if sees the pawn
    GetWorldTimerManager().SetTimer(FireTimer, this, &AAIReploid::SpawnProjectile, FireRate, true);
    //SpawnProjectile();
}

void AAIReploid::NoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
    HeardLocation = Location - GetActorLocation();
    HeardLocation.Normalize();
    FRotator newlookat = FRotationMatrix::MakeFromX(HeardLocation).Rotator();
    newlookat.Pitch = 0.0f;
    newlookat.Roll = 0.0f;
    SetActorRotation(newlookat);
}

void AAIReploid::SpawnProjectile()
{
    if (ProjectileClass != nullptr)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            FTransform SpawTransform = Muzzle->GetComponentTransform();
            AProjectile* ProjectileSpawn = World->SpawnActor<AProjectile>(ProjectileClass, SpawTransform, SpawnParams);
            if (ProjectileSpawn)
            {
                FVector direction = FRotationMatrix(SpawTransform.Rotator()).GetScaledAxis(EAxis::X);
                ProjectileSpawn->FireDirection(direction);
                ProjectileSpawn->SetOwner(this);
            }
        }
    }
}

// Called every frame
void AAIReploid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TargetActor)
    {
        if (AiCapsule->GetComponentLocation().X > TargetActor->GetActorLocation().X)
        {
            MoveRight(-1.0f);
        }
        else if (AiCapsule->GetComponentLocation().X < TargetActor->GetActorLocation().X)
        {
            MoveRight(1.0f);
        }
        else {
            MoveRight(0.0f);
        }
    }
    if (!CurrentVelocity.IsZero())
    {
        FVector newloc = GetActorLocation() + (CurrentVelocity * DeltaTime);
        SetActorLocation(newloc);
    }
}

// Called to bind functionality to input
void AAIReploid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIReploid::MoveRight(float AxisValue)
{
    CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 300.0f;
}

