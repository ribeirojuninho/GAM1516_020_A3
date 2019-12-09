// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "XGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMANX_API AXGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:

    UPROPERTY(VisibleDefaultsOnly, Category = "Config")
        int32 Life = 1;
    UFUNCTION(BlueprintCallable, Category = "Pickups")
        int GetLife();

};
