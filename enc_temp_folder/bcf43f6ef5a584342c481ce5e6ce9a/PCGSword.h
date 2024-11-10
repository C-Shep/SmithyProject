// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PCGSword.generated.h"

UCLASS()
class SMITHY_API APCGSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCGSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	//Size of the blade
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector bladeCubeRadius;

	//Size of the guard
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector guardCubeRadius;

	float width;
	float girth;

	//Default Attributes
	TArray<FVector> vertices;
	TArray<int> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> tangents;

	//Blade Attributes
	TArray<FVector> bladeVertices;
	TArray<int> bladeTriangles;
	TArray<FVector> bladeNormals;
	TArray<FVector2D> bladeUvs;
	TArray<FLinearColor> bladeVertexColors;
	TArray<FProcMeshTangent> bladeTangents;

	//Guard Attributes
	TArray<FVector> guardVertices;
	TArray<int> guardTriangles;
	TArray<FVector> guardNormals;
	TArray<FVector2D> guardUvs;
	TArray<FLinearColor> guardVertexColors;
	TArray<FProcMeshTangent> guardTangents;

	void GenerateMesh();

	void MeshReset();

	void GenerateBlade();
	void GenerateGuard();
	void GenerateGrip();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* sceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* blade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* guard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* grip;

	void AddTriangleMesh(FVector topRight, FVector bottomRight, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent);
	void AddQuadMesh(FVector topRight, FVector bottomRight, FVector topLeft, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent);

};
