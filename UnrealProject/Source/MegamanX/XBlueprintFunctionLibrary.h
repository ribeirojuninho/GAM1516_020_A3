// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMANX_API UXBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    UXBlueprintFunctionLibrary();

    static void LockPhysicsTo2DAxis(class UPrimitiveComponent* primitive);
};
