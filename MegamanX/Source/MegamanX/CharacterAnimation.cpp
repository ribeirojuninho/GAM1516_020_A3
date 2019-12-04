// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimation.h"
#include "Classes/PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbook.h"
#include "ConstructorHelpers.h"

ACharacterAnimation::ACharacterAnimation()
{
    UPaperSpriteComponent* component = Cast<UPaperSpriteComponent>(GetComponentByClass(UPaperSpriteComponent::StaticClass()));
    if (component)
    {
        component->DestroyComponent();
        component->SetActive(false);
    }
    MegamanXAnimation = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);

    if (MegamanXAnimation)
    {
        MegamanXAnimation->SetupAttachment(RootComponent);
        MegamanXAnimation->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MegamanXAnimation->SetGenerateOverlapEvents(false);
    }
    SetState(EAnimState::Idle);
}


void ACharacterAnimation::MoveRight(float value)
{
    Super::MoveRight(value);

    if (MegamanXAnimation)
    {
        if (TravelDirection < 0.0f)
        {
            MegamanXAnimation->SetRelativeRotation(FRotator(FRotator::ZeroRotator));
        }
        else if (TravelDirection > 0.0f)
        {
            MegamanXAnimation->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
        }

        if (AnimState == EAnimState::Idle || AnimState == EAnimState::Walking)
        {
            if (TravelDirection == 0.0f)
            {
                SetState(EAnimState::Idle);
            }
            else
            {
                /*if (OnFloor)
                {
                    if()
                }*/

                SetState(EAnimState::Walking);
            }
            
        }
    }
}

void ACharacterAnimation::BeginPlay()
{
    Super::BeginPlay();
    if (MegamanXAnimation != nullptr)
    {
        MegamanXAnimation->Stop();
    }
}

void ACharacterAnimation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateAnimation();
}

void ACharacterAnimation::UpdateAnimation()
{
    float Velocity = GetVelocity().Size();

    if (Velocity > 0)
    {
        if (AnimState == EAnimState::Jumping)
        {
            MegamanXAnimation->SetFlipbook(JumpFlipbook);
            MegamanXAnimation->SetLooping(false);
            MegamanXAnimation->Play();
        }
        else
        {
            MegamanXAnimation->SetFlipbook(WalkingFlipbook);
            MegamanXAnimation->SetLooping(true);
            MegamanXAnimation->Play();
        }
    }
    else
    {
        MegamanXAnimation->Stop();
        MegamanXAnimation->SetFlipbook(IdleFlipbook);
        MegamanXAnimation->SetLooping(true);
        MegamanXAnimation->Play();
    }

    
}

void ACharacterAnimation::Jump()
{
    /* if (OnFloor)
     {

     }*/
    Super::Jump();
    if (AnimState == EAnimState::Walking || AnimState == EAnimState::Idle)
    {
        SetState(EAnimState::Jumping);
    }
}

void ACharacterAnimation::Land()
{
    /*if (!WasOnFloor)
    {

    }*/
    Super::Land();
    if (AnimState == EAnimState::Dead)
    {
        SetState(EAnimState::Dead);
    }
    else
    {
        SetState(EAnimState::Idle);
    }
}

void ACharacterAnimation::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}
