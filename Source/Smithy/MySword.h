// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCGCube.h"
#include "MySword.generated.h"

/**
 * 
 */
UCLASS()
class SMITHY_API AMySword : public AActor// : public APCGSword
{
	GENERATED_BODY()
public:
	AMySword();

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	APCGCube* pcgBlade;
	APCGCube* pcgGuard;
};
