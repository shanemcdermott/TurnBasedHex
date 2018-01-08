// Fill out your copyright notice in the Description page of Project Settings.

#include "GloomHUD.h"




AGloomHUD::AGloomHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void AGloomHUD::PrepareForRound()
{
	Execute_OnPrepareForRound(this);
}

void AGloomHUD::BeginTurn_Implementation()
{

}

void AGloomHUD::EndTurn_Implementation()
{

}