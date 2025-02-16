// Fill out your copyright notice in the Description page of Project Settings.
#include "SwordSpawner.h"



// Sets default values
ASwordSpawner::ASwordSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//init vars
	heightSlider = 100.f;

	heightVariety = 75.f;

	widthSlider = 25.f;

	widthVariety = 20.f;

	isPrismBlade = false;
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

		//Start the Deferred Spawn
		APCGSword* mySword = World->SpawnActorDeferred<APCGSword>(swordBpClass, GetActorTransform(),GetOwner()); //Spawn Sword

		//Blade Height Calculations & Clamping
		float finalBladeHeightMin = heightSlider - heightVariety;
		float finalBladeHeightMax = heightSlider + heightVariety;

		if (finalBladeHeightMin < 1.f) finalBladeHeightMin = 1.f;
		if (finalBladeHeightMax > 200.f) finalBladeHeightMax = 200.f;

		//Blade Width Calculations & Clamping
		float finalBladeWidthMin = widthSlider - widthVariety;
		float finalBladeWidthMax = widthSlider + widthVariety;

		if (finalBladeWidthMin < 1.f) finalBladeWidthMin = 1.f;
		if (finalBladeWidthMax > 50.f) finalBladeWidthMax = 50.f;

		//Set the blades attributes
		mySword->SetBladeAttributes(finalBladeHeightMin, finalBladeHeightMax, finalBladeWidthMin, finalBladeWidthMax, isPrismBlade);

		//Stop the Deferred Spawn, Actually spawn the sword now
		UGameplayStatics::FinishSpawningActor(mySword, mySword->GetTransform());
	}
	
}

// Called every frame
void ASwordSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

