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

	dummyHp = 1000;
	dummyHpMax = 1000;

	canBeHit = true;

	dead = false;
	deadTimer = -1;
	deadTimerMax = 3.f;
}

void ADummy::TakeDamage(int32 damage)
{
	float tenPercentDamage = ceilf(damage * 0.1);

	int32 extraDmg = FMath::RandRange(-tenPercentDamage, tenPercentDamage);

	dummyHp -= (damage + extraDmg);

	if (dummyHp<=0)
	{
		dead = true;
		deadTimer = deadTimerMax;
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

	if (dead)
	{
		if (deadTimer > 0.f)
		{
			deadTimer -= DeltaTime;
		}
		else {
			dummyHp = dummyHpMax;
			dead = false;
		}
	}
}


