// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimation.h"
#include "Classes/PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbook.h"
#include "ConstructorHelpers.h"
#include <Components/ArrowComponent.h>
#include "Components/AudioComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"

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
        if (value > 0.0f)
        {
            MegamanXAnimation->SetRelativeRotation(FRotator(FRotator::ZeroRotator));
            DirectionEnum = EDirectionEnum::DE_Right;
            
        }
        else if (value < 0.0f)
        {
            MegamanXAnimation->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
            DirectionEnum = EDirectionEnum::DE_Left;
        }

        if (DirectionEnum == EDirectionEnum::DE_Right)
        {
            Muzzle->SetRelativeRotation(FRotator(FRotator::ZeroRotator));
            Muzzle->SetRelativeLocation(FVector(70.0f, 0.0f, 5.0f));
        }
        else
        {
            Muzzle->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
            Muzzle->SetRelativeLocation(FVector(-70.0f, 0.0f, 5.0f));
        }


        if (AnimState == EAnimState::Idle || AnimState == EAnimState::Walking || AnimState == EAnimState::Dashing)
        {
            if (value == 0.0f)
            {
                if (dashing)
                {
                    SetState(EAnimState::Dashing);
                }
                else
                {
                    SetState(EAnimState::Idle);
                }
                
            }
            else
            {
                if (OnFloor)
                {
                    if (!AudioComponent->IsPlaying())
                    {
                        AudioComponent->SetSound(WalkSound);
                        MakeNoise(1.0f, this, GetActorLocation());
                        AudioComponent->Play();

                    }
                }
                if (dashing)
                {
                    SetState(EAnimState::Dashing);
                } 
                else
                {
                    SetState(EAnimState::Walking);
                }
                
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
   //float Velocity = GetVelocity().Size();

    if (CurrentVelocity.Size() > 0)
    {
        if (AnimState == EAnimState::Jumping)
        {
            MegamanXAnimation->SetFlipbook(JumpFlipbook);
            MegamanXAnimation->SetLooping(false);
            MegamanXAnimation->Play();
        }
        else if (AnimState == EAnimState::Shooting)
        {
            MegamanXAnimation->SetFlipbook(WalkingAndFireFlipbook);
            MegamanXAnimation->SetLooping(true);
            MegamanXAnimation->Play();
        }
        else if (AnimState == EAnimState::Dashing)
        {
            MegamanXAnimation->SetFlipbook(DashFlipbook);
            MegamanXAnimation->SetLooping(true);
            MegamanXAnimation->Play();
        }
        else
        {
            MegamanXAnimation->SetFlipbook(WalkingFlipbook);
            MegamanXAnimation->SetLooping(true);
            MegamanXAnimation->Play();
        }
    }

    else if (CurrentVelocity.Size() <= 0)
    {
       if(AnimState == EAnimState::Shooting)
       {
        MegamanXAnimation->Stop();
        MegamanXAnimation->SetFlipbook(FireFlipbook);
        MegamanXAnimation->SetLooping(true);
        MegamanXAnimation->Play();
       }
       else  if (AnimState == EAnimState::Dashing)
       {
           MegamanXAnimation->Stop();
           MegamanXAnimation->SetFlipbook(FireFlipbook);
           MegamanXAnimation->SetLooping(true);
           MegamanXAnimation->Play();
       }
       else
       {
           MegamanXAnimation->Stop();
           MegamanXAnimation->SetFlipbook(IdleFlipbook);
           MegamanXAnimation->SetLooping(true);
           MegamanXAnimation->Play();
       }
    }

}

void ACharacterAnimation::Jump()
{
     if (OnFloor)
     {
         AudioComponent->SetSound(JumpSound);
         MakeNoise(1.0f, this, GetActorLocation());
         AudioComponent->Play();
     }
    Super::Jump();
    if (AnimState == EAnimState::Walking || AnimState == EAnimState::Idle || AnimState == EAnimState::Dashing)
    {
        SetState(EAnimState::Jumping);
    }
}

void ACharacterAnimation::Land()
{
    Super::Land();

    if (AnimState == EAnimState::Shooting)
    {
        SetState(EAnimState::Shooting);
    }
    else
    {
        SetState(EAnimState::Idle);
    }
}

void ACharacterAnimation::Dash()
{
    if (OnFloor)
    {
        AudioComponent->SetSound(DashSound);
        MakeNoise(1.0f, this, GetActorLocation());
        AudioComponent->Play();
    }
    Super::Dash();

        SetState(EAnimState::Dashing);
    
}

void ACharacterAnimation::StopDash()
{
    Super::StopDash();
    SetState(EAnimState::Idle);
}

void ACharacterAnimation::XBuster()
{
    Super::XBuster();
    SetState(EAnimState::Shooting);
}

void ACharacterAnimation::ReleaseBuster()
{
    Super::ReleaseBuster();
    SetState(EAnimState::Idle);
}


void ACharacterAnimation::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ACharacterAnimation::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

