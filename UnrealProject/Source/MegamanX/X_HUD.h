// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "X_HUD.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMANX_API AX_HUD : public AHUD
{
	GENERATED_BODY()
	
protected:

    virtual void BeginPlay() override;

public:

    AX_HUD();

    UPROPERTY(EditAnywhere, Category = "X_Status")
        class TSubclassOf<class UUserWidget> StatusGuiClass;

   UPROPERTY()
        class UUserWidget* StatusGui;

    
};
