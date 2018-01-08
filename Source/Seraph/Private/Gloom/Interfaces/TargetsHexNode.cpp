// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetsHexNode.h"


UTargetsHexNode::UTargetsHexNode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{}
// Add default functionality here for any IScenarioInterface functions that are not pure virtual.


void ITargetsHexNode::SetTarget(AActor* NewTarget)
{
}

AActor* ITargetsHexNode::GetTarget()
{
	return nullptr;
}


