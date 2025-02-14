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

	UPROPERTY(EditAnywhere)
	float heightSlider;

	UPROPERTY(EditAnywhere)
	float heightVariety;

	UPROPERTY(EditAnywhere)
	float widthSlider;

	UPROPERTY(EditAnywhere)
	float widthVariety;

	UPROPERTY(EditAnywhere)
	bool isPrismBlade;

	AActor* spawnedSword;

};
