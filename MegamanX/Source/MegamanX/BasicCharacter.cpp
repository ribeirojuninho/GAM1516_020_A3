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

    //Setting collision and onHit
    CapsuleComponent->SetSimulatePhysics(true);
    CapsuleComponent->SetNotifyRigidBodyCollision(true);
    CapsuleComponent->OnComponentHit.AddDynamic(this, &ABasicCharacter::OnHit);

    //Capsule can rotate on Z (turn left or right) and move translate on X and Z Axis
    CapsuleComponent->SetConstraintMode(EDOFMode::SixDOF);
    CapsuleComponent->BodyInstance.bLockXRotation = true;
    CapsuleComponent->BodyInstance.bLockYRotation = true;
    CapsuleComponent->BodyInstance.bLockZRotation = false;
    CapsuleComponent->BodyInstance.bLockYTranslation = true;

    //CapsuleComponent as RootComponent
    SetRootComponent(CapsuleComponent);



    PlayerSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("PlayerSprite Component");
    static FName CollisionProfileName(TEXT("CharacterMesh"));
    PlayerSpriteComponent->SetCollisionProfileName(CollisionProfileName);
    PlayerSpriteComponent->SetGenerateOverlapEvents(false);
    PlayerSpriteComponent->SetupAttachment(RootComponent);

    /* static ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSpriteTemplate(TEXT("/Game/SpriteAssets/Player/mario_Sprite"));
     if (PaperSpriteTemplate.Succeeded())
     {
         PlayerSpriteComponent->SetSprite(PaperSpriteTemplate.Object);
     }*/

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
    SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
    SpringArmComponent->TargetArmLength = 500.0f;
    SpringArmComponent->bEnableCameraLag = true;
    SpringArmComponent->CameraLagSpeed = 10.0f;
    SpringArmComponent->bInheritYaw = true;
    SpringArmComponent->bAbsoluteRotation = true;


    FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>("Player Camera Component");
    FollowCameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    FollowCameraComponent->OrthoWidth = 2048.0f;
    FollowCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    FollowCameraComponent->bUsePawnControlRotation = false;
    FollowCameraComponent->bAutoActivate = true;


    
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();

    OnFloor = false;
    WasOnFloor = false;
	
}


// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (!OnFloor)
    {
        WasOnFloor = false;
    }
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveRight", this, &ABasicCharacter::MoveRight);
    PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &ABasicCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABasicCharacter::Jump);
}


void ABasicCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != nullptr)
    {
        if (OtherActor->ActorHasTag(FName("Floor")))
        {
            Land();
        }
     }
}

void ABasicCharacter::MoveRight(float value)
{
    if (value != 0)
    {
        const FVector PlayerVelocity = GetVelocity();
        TravelDirection = -PlayerVelocity.X;

        if (PlayerSpriteComponent) 
        {
            if (TravelDirection < 0.0f)
            {
                PlayerSpriteComponent->SetRelativeRotation(FRotator(FRotator::ZeroRotator));
            }
            else if (TravelDirection > 0.0f)
            {
                PlayerSpriteComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
            }
        }

        if (OnFloor)
        {
            FVector currentVelocity = CapsuleComponent->BodyInstance.GetUnrealWorldVelocity();

            if (FMath::Abs(currentVelocity.X) < MaxVelX)
            {
                CapsuleComponent->BodyInstance.AddForce(FVector(1.0, 0.0f, 0.0f) * ForceScalarX * value);
            }
            else
            {
                FVector newVelocity = FVector(MaxVelX * value, 0.0f, currentVelocity.Z);
                CapsuleComponent->BodyInstance.SetLinearVelocity(newVelocity, false);
            }
        }
        else
        {
            CapsuleComponent->BodyInstance.AddForce(FVector(1.0f, 0.0f, 0.0f) * AirborneScalarX * value);
        }
    }
    else
    {
        TravelDirection = 0.0f;
    }
}

void ABasicCharacter::Jump()
{
    if (OnFloor) {
        OnFloor = false;
        CapsuleComponent->BodyInstance.AddForce(FVector(0.0f, 0.0f, 1.0f) * ForceScalarZ * JumpPowerZ);
    }
}

void ABasicCharacter::Land()
{
    OnFloor = true;
    if (!WasOnFloor) {
        WasOnFloor = true;

    }
}

