
#include "HexNode.h"
#include "UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AHexNode::AHexNode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
}

void AHexNode::SetColor(const FLinearColor& InColor)
{
	if (NodeMaterial)
	{
		NodeColor = InColor;
		NodeMaterial->SetVectorParameterValue(FName("Color"), NodeColor);
	}
}

void AHexNode::SetOpacity(float InOpacity)
{
	if (NodeMaterial)
	{
		NodeOpacity = InOpacity;
		NodeMaterial->SetScalarParameterValue(FName("Opacity"), NodeOpacity);
	}
}
