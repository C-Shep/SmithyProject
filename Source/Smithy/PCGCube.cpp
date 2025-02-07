// Fill out your copyright notice in the Description page of Project Settings.


#include "PCGCube.h"

// Sets default values
APCGCube::APCGCube()
{
	//srand(time(NULL));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create procedural mesh component, set it to the root component
	blade = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("BladeMesh"));
	RootComponent = blade;

	blade->bUseAsyncCooking = true;

	//Randomize the swords size parameters
	//X = left and right	(Width)
	//Y = forward and back	(Girth)
	//Z = height			(Height)

	//Size of the cube at base
	float cubeSizeMin = 5.f;
	float cubeSizeMax = 50.f;
	float randCubeSize = FMath::RandRange(cubeSizeMin, cubeSizeMax);

	//Height
	float heightMin = 25.f;
	float heightMax = 200.f;
	float randHeight = FMath::RandRange(heightMin, heightMax);

	//Set size of cube to be the randomized parameters
	cubeRadius = FVector(randCubeSize, randCubeSize, randHeight);

	/// --- Width and Girth for later use after mesh construction, its just here for consistensy sake ---

	//Width
	//Set random width for use later after the cube is created
	float widthMin = 0.2f;
	float widthMax = 1.5f;
	width = FMath::RandRange(widthMin, widthMax);

	//Girth
	//Set random girth for use later after the cube is created
	float girthMin = widthMin;
	float girthMax = width - (width / 4); //WE GOTS A MAGIC NUMBA HERE BAWSS!!! (wait does this even count as a magic number?)
	girth = FMath::RandRange(girthMin, girthMax);
}

// Called when the game starts or when spawned
void APCGCube::BeginPlay()
{
	Super::BeginPlay();
}

void APCGCube::PostActorCreated()
{
	//Generate Mesh when created in level
	Super::PostActorCreated();
	GenerateMesh();
}

void APCGCube::PostLoad()
{
	//Generate Mesh when loaded in level
	Super::PostLoad();
	GenerateMesh();
}

// Called every frame
void APCGCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCGCube::GenerateMesh()
{
	//Reset all attributes. Good Practice.
	vertices.Reset();
	triangles.Reset();
	normals.Reset();
	uvs.Reset();
	vertexColors.Reset();
	tangents.Reset();

	int32 triangleIndexCount = 0;
	FVector definedShape[8];
	FProcMeshTangent tangentSetup;

	const FRotator rot(0,45,0);

	definedShape[0] = rot.RotateVector(FVector(-cubeRadius.X, cubeRadius.Y, cubeRadius.Z));	//Forward Top Right
	definedShape[1] = rot.RotateVector(FVector(-cubeRadius.X, cubeRadius.Y, -cubeRadius.Z));	//Forward Bottom Right
	definedShape[2] = rot.RotateVector(FVector(-cubeRadius.X, -cubeRadius.Y, cubeRadius.Z));	//Forward Top Left
	definedShape[3] = rot.RotateVector(FVector(-cubeRadius.X, -cubeRadius.Y, -cubeRadius.Z));	//Forward Bottom Left

	definedShape[4] = rot.RotateVector(FVector(cubeRadius.X, -cubeRadius.Y, cubeRadius.Z));	//Reverse Top Right
	definedShape[5] = rot.RotateVector(FVector(cubeRadius.X, -cubeRadius.Y, -cubeRadius.Z));	//Reverse Bottom Right
	definedShape[6] = rot.RotateVector(FVector(cubeRadius.X, cubeRadius.Y, cubeRadius.Z));	//Reverse Top Left
	definedShape[7] = rot.RotateVector(FVector(cubeRadius.X, cubeRadius.Y, -cubeRadius.Z));	//Reverse Bottom Left

	//Front
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddQuadMesh(definedShape[0], definedShape[1], definedShape[2], definedShape[3], triangleIndexCount, tangentSetup);

	//Left
	tangentSetup = FProcMeshTangent(1.f, 0.0f, 0.0f);
	AddQuadMesh(definedShape[2], definedShape[3], definedShape[4], definedShape[5], triangleIndexCount, tangentSetup);

	//Back
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddQuadMesh(definedShape[4], definedShape[5], definedShape[6], definedShape[7], triangleIndexCount, tangentSetup);

	//Right
	tangentSetup = FProcMeshTangent(-1.f, 0.0f, 0.0f);
	AddQuadMesh(definedShape[6], definedShape[7], definedShape[0], definedShape[1], triangleIndexCount, tangentSetup);

	//Top
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddQuadMesh(definedShape[6], definedShape[0], definedShape[4], definedShape[2], triangleIndexCount, tangentSetup);

	//Bottom
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddQuadMesh(definedShape[1], definedShape[7], definedShape[3], definedShape[5], triangleIndexCount, tangentSetup);

	//Create the actual mesh from the multiple quad meshes
	blade->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, true);

	//Modify the Blade's Transform to look... uh... blade-like
	//blade->SetWorldRotation(FRotator(0.f, 90.f,0.f));
	blade->SetRelativeScale3D(FVector(girth, width, 1));
}

void APCGCube::AddTriangleMesh(FVector topRight, FVector bottomRight, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent)
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
	for (int i = 0; i < 3; i++)
	{
		normals.Add(thisNorm);
		tangents.Add(tangent);
		vertexColors.Add(FLinearColor::Red);
	}

	uvs.Add(FVector2D(1.0f, 1.0f));//Top Right
	uvs.Add(FVector2D(1.0f, 0.0f));//Bottom Right
	uvs.Add(FVector2D(0.0f, 0.0f));//Bottom Left
}

void APCGCube::AddQuadMesh(FVector topLeft, FVector bottomLeft, FVector topRight, FVector bottomRight, int32& triIndex, FProcMeshTangent tangent)
{
	//vertices
	vertices.Add(topRight);
	vertices.Add(bottomRight);
	vertices.Add(topLeft);
	vertices.Add(bottomLeft);

	//triangles points
	int32 point1 = triIndex++;
	int32 point2 = triIndex++;
	int32 point3 = triIndex++;
	int32 point4 = triIndex++;

	//triangles
	triangles.Add(point1);
	triangles.Add(point2);
	triangles.Add(point3);

	triangles.Add(point4);
	triangles.Add(point3);
	triangles.Add(point2);

	//normals cross product
	FVector thisNorm = FVector::CrossProduct(topLeft - bottomRight, topLeft - topRight).GetSafeNormal();

	//normals, tangent and colour
	//4 for quad
	for (int i = 0; i < 4; i++)
	{
		normals.Add(thisNorm);
		tangents.Add(tangent);
		vertexColors.Add(FLinearColor::Red);
	}

	//uvs
	//some uvs are upside down i gotta fix this
	uvs.Add(FVector2D(0.0f, 1.0f));//Top Left
	uvs.Add(FVector2D(0.0f, 0.0f));//Bottom Left
	uvs.Add(FVector2D(1.0f, 1.0f));//Top Right
	uvs.Add(FVector2D(1.0f, 0.0f));//Bottom Right
}

