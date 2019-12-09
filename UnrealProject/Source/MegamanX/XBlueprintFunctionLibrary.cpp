// Fill out your copyright notice in the Description page of Project Settings.


#include "XBlueprintFunctionLibrary.h"
#include "Components/PrimitiveComponent.h"



UXBlueprintFunctionLibrary::UXBlueprintFunctionLibrary()
{

}

void UXBlueprintFunctionLibrary::LockPhysicsTo2DAxis(class UPrimitiveComponent* primitive)
{
    primitive->GetBodyInstance()->bLockRotation = true;
    primitive->GetBodyInstance()->bLockXRotation = true;
    primitive->GetBodyInstance()->bLockYRotation = true;
    primitive->GetBodyInstance()->bLockZRotation = false;
    primitive->GetBodyInstance()->bLockYTranslation = true;
}
