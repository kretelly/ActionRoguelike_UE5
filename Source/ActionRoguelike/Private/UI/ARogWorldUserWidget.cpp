// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ARogWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "GameFramework/Character.h"


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

	// If fail project to screen, this hide the widget, very useful whe the other actor is not in our field vision
	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bProjectionSucceed ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	/** NOTE:
	* 
	* If we hide the whole widget using SetVibility, and not only a child of it, the tick will stop and only get called again when it is setted to True again.
	*
	*/
}
