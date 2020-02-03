// Fill out your copyright notice in the Description page of Project Settings.


#include "X_HUD.h"
#include "Blueprint/UserWidget.h"
#include "ConstructorHelpers.h"

AX_HUD::AX_HUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> XHud(TEXT("'/Game/Blueprints/UI/Status'"));
    if (XHud.Succeeded())
    {
        StatusGuiClass = XHud.Class;
    }
  
}

void AX_HUD::BeginPlay()
{
    StatusGui = CreateWidget<UUserWidget>(GetGameInstance(), StatusGuiClass);
    StatusGui->AddToViewport();

    
}
