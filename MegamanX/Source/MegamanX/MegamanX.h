// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAnimState : uint8
{
    Idle,
    Jumping,
    Walking,
    Shooting,
    Dashing,
    Damage,
    Dying,
    Dead

};
