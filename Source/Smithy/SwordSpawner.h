// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCGSword.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SwordSpawner.generated.h"

UCLASS()
class SMITHY_API ASwordSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APCGSword> swordBpClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float heightSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float heightVariety;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float widthSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float widthVariety;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float guardWidthSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float guardWidthVariety;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float gripHeightSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float gripHeightVariety;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pommelSizeSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pommelSizeVariety;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int bladeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 matType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float curveSizeSlider;

	AActor* spawnedSword;

};
