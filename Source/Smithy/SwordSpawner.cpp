// Fill out your copyright notice in the Description page of Project Settings.
#include "SwordSpawner.h"



// Sets default values
ASwordSpawner::ASwordSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASwordSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())	//if the world exists
	{
		FActorSpawnParameters SpawnInfo;
		FVector swordSpawnPos = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);	//Get spawn point
		//APCGSword* mySword = World->SpawnActor<APCGSword>(swordBpClass, swordSpawnPos, GetActorRotation(), SpawnInfo); //Spawn Sword
		APCGSword* mySword = World->SpawnActorDeferred<APCGSword>(swordBpClass, GetActorTransform(),GetOwner()); //Spawn Sword
		mySword->SetBladeAttributes(heightSlider-1.f, heightSlider+1.f);
		UGameplayStatics::FinishSpawningActor(mySword, mySword->GetTransform());
	}
	
}

// Called every frame
void ASwordSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

