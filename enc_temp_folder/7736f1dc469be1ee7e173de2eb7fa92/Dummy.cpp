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

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	collisionBox->SetupAttachment(sceneComponent);
	collisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

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

	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADummy::OverlapBegin);
	collisionBox->OnComponentEndOverlap.AddDynamic(this, &ADummy::OverlapEnd);

}

void ADummy::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("Sword") && canBeHit)
	{
		APCGSword* thisSword = Cast<APCGSword>(OtherActor);
		if(thisSword != nullptr){
			TakeDamage(thisSword->swordDamage);
			canBeHit = false;
		}
	}
}

void ADummy::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Sword") && canBeHit == false)
	{
		canBeHit = true;
	}
}



// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


