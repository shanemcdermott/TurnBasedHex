// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomClassWidget.h"
#include "Gloom/Player/GloomPlayerController.h"



void UGloomClassWidget::SubmitPawnSelection()
{
	AGloomPlayerController* PC = Cast<AGloomPlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->SetPawnClass(PawnClass);
	}
}
