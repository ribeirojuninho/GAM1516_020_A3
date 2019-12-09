// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Classes/PaperSpriteComponent.h"
#include "Classes/PaperSprite.h"
#include "ConstructorHelpers.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/Character.h"
#include <Components/ArrowComponent.h>
#include "Projectile.h"
#include "PickupItem.h"
#include "XBlueprintFunctionLibrary.h"
#include "XGameStateBase.h"
#include "Components/AudioComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "EngineUtils.h"
#include "Floor.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //Use only Yaw from the controller and ignore the rest of the rotation.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(ACharacter::CapsuleComponentName);
    CapsuleComponent->InitCapsuleSize(55.0f, 75.0f);
    CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    CapsuleComponent->CanCharacterStepUpOn = ECB_Yes;
    CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
    CapsuleComponent->SetCanEverAffectNavigation(false);
    CapsuleComponent->bDynamicObstacle = true;
    CapsuleComponent->SetSimulatePhysics(true);
    CapsuleComponent->SetNotifyRigidBodyCollision(true);
    CapsuleComponent->OnComponentHit.AddDynamic(this, &ABasicCharacter::OnHit);
    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ABasicCharacter::OnOverlapBegin);
    CapsuleComponent->SetConstraintMode(EDOFMode::SixDOF);
    UXBlueprintFunctionLibrary::LockPhysicsTo2DAxis(CapsuleComponent);
    SetRootComponent(CapsuleComponent);

    PlayerSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("PlayerSprite Component");
    static FName CollisionProfileName(TEXT("CharacterMesh"));
    PlayerSpriteComponent->SetCollisionProfileName(CollisionProfileName);
    PlayerSpriteComponent->SetGenerateOverlapEvents(false);
    PlayerSpriteComponent->SetupAttachment(RootComponent);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
    SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
    SpringArmComponent->TargetArmLength = 500.0f;
    SpringArmComponent->bEnableCameraLag = true;
    SpringArmComponent->CameraLagSpeed = 4.0f;
    SpringArmComponent->bInheritYaw = true;
    SpringArmComponent->bAbsoluteRotation = true;
    SpringArmComponent->bDoCollisionTest = false;

    FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>("Player Camera Component");
    FollowCameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    FollowCameraComponent->OrthoWidth = 2048.0f;
    FollowCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    FollowCameraComponent->bUsePawnControlRotation = false;
    FollowCameraComponent->bAutoActivate = true;

    Muzzle = CreateDefaultSubobject<UArrowComponent>("Muzzle");
    Muzzle->SetupAttachment(RootComponent);

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    AudioComponent->SetupAttachment(RootComponent);

    NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
    
    Tags.Add("Player");

    DirectionEnum = EDirectionEnum::DE_Right;
}

FVector ABasicCharacter::GetMuzzleLocation() const
{
    return Muzzle->GetComponentToWorld().GetLocation();
}


// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
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

    OnFloor = false;
    WasOnFloor = false;

    if (AXGameStateBase* GSRef = Cast<AXGameStateBase>(GetWorld()->GetGameState()))
    {
        XGameStateBase = GSRef;
    }

}


// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (!OnFloor)
    {
        WasOnFloor = false;
    }

    if (!CurrentVelocity.IsZero())
    {
        FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
        SetActorLocation(NewLocation);
    }
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveRight", this, &ABasicCharacter::MoveRight);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasicCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABasicCharacter::Jump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABasicCharacter::XBuster);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABasicCharacter::ReleaseBuster);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABasicCharacter::Dash);
    PlayerInputComponent->BindAction("Dash", IE_Released, this, &ABasicCharacter::StopDash);
}

void ABasicCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != nullptr))
    {
        if (OtherComp->ComponentHasTag(FName("Floor")) || OtherComp->ComponentHasTag(FName("SecondFloor")) || OtherActor->ActorHasTag("MovingPlat") || OtherActor->ActorHasTag("AiPlayer"))
        {
            Land();
        }
    }
}

void ABasicCharacter::MoveRight(float value)
{
    if (value != 0)
    {
       if (OnFloor)
       {
           CurrentVelocity.X = FMath::Clamp(value, -1.0f, 1.0f) * 600.0f;
       }
       else
       {
           CurrentVelocity.X = FMath::Clamp(value, -1.0f, 1.0f) * 450.0f;
       }
       if (value > 0)
       {
           DirectionEnum = EDirectionEnum::DE_Right;
       } 
       else
       {
           DirectionEnum = EDirectionEnum::DE_Left;
       }
    }
    else if (!dashing)
    {
        CurrentVelocity.X = 0.0f;
    }
    
}

void ABasicCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Pickup"))
    {
        APickupItem* item = Cast<APickupItem>(OtherActor);
        item->Destroy();
        XGameStateBase->Life++;
        AudioComponent->SetSound(LifeSound);
        AudioComponent->Play();
    }

    if (OtherComp->ComponentHasTag(FName("OverlapFloor")))
    {
        FVector checkpointloc = field->Checkpoint->GetComponentLocation();
        SetActorLocation(checkpointloc);
    }


}

void ABasicCharacter::Jump()
{
    if (OnFloor) {
        OnFloor = false;
        CurrentVelocity.Z = 1 * 800.0f;
        
    }
}

void ABasicCharacter::Land()
{
    OnFloor = true;
    if (!WasOnFloor) {
        WasOnFloor = true;
        
        CurrentVelocity.Z = 0.0f;
    }
}

void ABasicCharacter::Dash()
{
    if (OnFloor) {
        if (DirectionEnum == EDirectionEnum::DE_Left)
        {
            CurrentVelocity.X = -1 * 800.0f;
            dashing = true;
        }
        else
        {
            CurrentVelocity.X = 1 * 800.0f;
            dashing = true;
        }
    }
}

void ABasicCharacter::StopDash()
{
    dashing = false;
}

void ABasicCharacter::SpawnProjectile()
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

void ABasicCharacter::Fire()
{
    if (OnFloor)
    {
        SpawnProjectile();
        if (!AudioComponent->IsPlaying())
        {
            AudioComponent->SetSound(FireSound);
            AudioComponent->Play();
        }
    }
 
}

void ABasicCharacter::XBuster()
{
    GetWorldTimerManager().SetTimer(FireTimer, this, &ABasicCharacter::Fire, FireRate, true);
  
}

void ABasicCharacter::ReleaseBuster()
{
    GetWorldTimerManager().ClearTimer(FireTimer);
    
}

