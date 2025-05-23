// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PCGCube.generated.h"

UCLASS()
class SMITHY_API APCGCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCGCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//Size of the blade
	FVector cubeRadius;

	float width;
	float girth;

	TArray<FVector> vertices;
	TArray<int> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> tangents;

	void GenerateMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* blade;

	void AddTriangleMesh(FVector topRight, FVector bottomRight, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent);
	void AddQuadMesh(FVector topRight, FVector bottomRight, FVector topLeft, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent);

};
