// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Dummy.generated.h"

UCLASS()
class SMITHY_API ADummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADummy();

	UFUNCTION() virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OverlapEnd(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeDamage(int32 damage);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int dummyHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* collisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* sceneComponent;

	bool canBeHit;
};
