// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ARogWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"

UARogWorldUserWidget::UARogWorldUserWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	//ParentCanvasPanel->AddChildToCanvas(ParentSizeBox);
	//ParentCanvasPanel->AddChild(ParentSizeBox);
}

void UARogWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor is no longer valid, removing Health Bar Widget."));
		RemoveFromParent();
		return;
	}

	/** Using ProjectWorldToScreen() we have to ajust the screen position
	FVector2D ScreenPosition;
	bool bProjectionSucceed = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);
	if (bProjectionSucceed)
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
	**/

	// Using ProjectWorldLocationToWidgetPosition() it ajusts the screen position by itself
	FVector2D ScreenPosition;
	bool bProjectionSucceed = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition, false); // Ajust it self
	if (bProjectionSucceed)
	{
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

}
