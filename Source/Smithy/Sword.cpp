// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	blade = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("BladeMesh"));
	RootComponent = blade;

	blade->bUseAsyncCooking = true;

}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
//	Super::BeginPlay();
	//CreateMesh();
}

void ASword::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateMesh();
}

void ASword::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword::CreateMesh()
{
	/*TArray<FVector> vertices;
	//vertices.Add(FVector(0, 0, 0));
	//vertices.Add(FVector(0, 100, 0));
	//vertices.Add(FVector(0, 0, 100));
	//vertices.Add(FVector(0, 100, 100));

	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(100, 0, 0));
	vertices.Add(FVector(0, 0, 100));
	vertices.Add(FVector(100, 0, 100));



	TArray<int> triangles;
	triangles.Add(0);
	triangles.Add(1);
	triangles.Add(2);

	triangles.Add(2);
	triangles.Add(1);
	triangles.Add(3);

	TArray<FVector> normals;
	normals.Add(FVector(0, 0, 0));
	normals.Add(FVector(0, 1, 0));
	normals.Add(FVector(0, 0, 1));
	normals.Add(FVector(0, 1, 1));

	TArray<FVector2D> uvs;
	uvs.Add(FVector2D(0,0));
	uvs.Add(FVector2D(10, 0));
	uvs.Add(FVector2D(0, 10));
	uvs.Add(FVector2D(10, 10));

	TArray<FLinearColor> vertexColors;
	vertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f));
	vertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f));
	vertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f));
	vertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f));

	TArray<FProcMeshTangent> tangents;
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	blade->CreateMeshSection_LinearColor(0,vertices,triangles,normals,uvs, vertexColors, tangents, true);

	blade->ContainsPhysicsTriMeshData(true);*/
}

void ASword::GenerateMesh()
{
	vertices.Reset();
	triangles.Reset();
	normals.Reset();
	uvs.Reset();
	vertexColors.Reset();
	tangents.Reset();

	int32 triangleIndexCount = 0;
	FVector definedShape[4];
	FProcMeshTangent tangentSetup;

	definedShape[0] = FVector(cubeRadius.X, cubeRadius.Y, cubeRadius.Z);	//Forward Top Right
	definedShape[1] = FVector(cubeRadius.X, cubeRadius.Y, -cubeRadius.Z);	//Forward Bottom Right
	definedShape[2] = FVector(cubeRadius.X, -cubeRadius.Y, cubeRadius.Z);	//Forward Top Left
	definedShape[3] = FVector(cubeRadius.X, -cubeRadius.Y, -cubeRadius.Z);	//Forward Bottom Left

	tangentSetup = FProcMeshTangent(0.f,1.0f,0.0f);

	//AddTriangleMesh(definedShape[0], definedShape[1], definedShape[2], triangleIndexCount, tangentSetup);
	AddQuadMesh(definedShape[0], definedShape[1], definedShape[2], definedShape[3], triangleIndexCount, tangentSetup);

	blade->CreateMeshSection_LinearColor(0,vertices,triangles,normals, uvs, vertexColors, tangents, true);
}

void ASword::AddTriangleMesh(FVector topRight, FVector bottomRight, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent) 
{
	int32 point1 = triIndex++;
	int32 point2 = triIndex++;
	int32 point3 = triIndex++;

	vertices.Add(topRight);
	vertices.Add(bottomRight);
	vertices.Add(bottomLeft);

	triangles.Add(point1);
	triangles.Add(point2);
	triangles.Add(point3);

	FVector thisNorm = FVector::CrossProduct(topRight, bottomRight).GetSafeNormal();
	for (int i=0; i < 3;i++)
	{
		normals.Add(thisNorm);
		tangents.Add(tangent);
		vertexColors.Add(FLinearColor::Red);
	}

	uvs.Add(FVector2D(1.0f, 1.0f));//Top Right
	uvs.Add(FVector2D(1.0f, 0.0f));//Bottom Right
	uvs.Add(FVector2D(0.0f, 0.0f));//Bottom Left
}

void ASword::AddQuadMesh(FVector topLeft, FVector bottomLeft, FVector topRight, FVector bottomRight, int32& triIndex, FProcMeshTangent tangent)
{
	int32 point1 = triIndex++;
	int32 point2 = triIndex++;
	int32 point3 = triIndex++;
	int32 point4 = triIndex++;

	vertices.Add(topRight);
	vertices.Add(bottomRight);
	vertices.Add(topLeft);
	vertices.Add(bottomLeft);

	triangles.Add(point1);
	triangles.Add(point2);
	triangles.Add(point3);

	triangles.Add(point4);
	triangles.Add(point3);
	triangles.Add(point2);

	FVector thisNorm = FVector::CrossProduct(topLeft - bottomRight, topLeft - topRight).GetSafeNormal();
	for (int i = 0; i < 4; i++)
	{
		normals.Add(thisNorm);
		tangents.Add(tangent);
		vertexColors.Add(FLinearColor::Red);
	}

	uvs.Add(FVector2D(0.0f, 1.0f));//Top Left
	uvs.Add(FVector2D(0.0f, 0.0f));//Bottom Left
	uvs.Add(FVector2D(1.0f, 1.0f));//Top Right
	uvs.Add(FVector2D(1.0f, 0.0f));//Bottom Right
}

