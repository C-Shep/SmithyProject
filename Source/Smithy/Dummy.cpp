// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"
#include "PCGSword.h"

// Sets default values
ADummy::ADummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = sceneComponent;

	dummyHp = 100;

	canBeHit = true;
}

void ADummy::TakeDamage(int32 damage)
{
	dummyHp -= damage;

	if (dummyHp<=0)
	{
		this->Destroy();
	}
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


