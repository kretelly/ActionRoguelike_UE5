// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARogWorldUserWidget.generated.h"

class USizeBox;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARogWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UARogWorldUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* ParentCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	
public:

	UPROPERTY(EditAnywhere, Category = UI)
	FVector WorldOffset;

	// meta = (ExposeOnSpawn = true) -> make this variable as a input pin at a moment we creat this widget via Blueprint
	UPROPERTY(BlueprintReadOnly, Category = UI, meta=(ExposeOnSpawn=true))
	AActor* AttachedActor;
};
