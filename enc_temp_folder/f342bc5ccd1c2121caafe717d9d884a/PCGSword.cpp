// Fill out your copyright notice in the Description page of Project Settings.


#include "PCGSword.h"

// Sets default values
APCGSword::APCGSword()
{
	//TO DO LIST
	//Proper randomisation of guard												DONE
	//Proper randomisation of grip												DONE
	//Add pommel																DONE
	//Redo generation functions cuz they could probably be a single function
	//Tips of swords
	//Redo Blade Variables cuz they suck
	//Different blade types, single edged, curved
	//Different guard types, curved, ornate, spiked
	//Different grip types, spear-like, round, ornate
	//Different pommel types, different shapes/designs


	//srand(time(NULL));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComponentRoot"));
	RootComponent = sceneComponent;

	//Create procedural mesh components, set up its attachments to the blade
	blade = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("BladeMesh"));
	guard = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GuardMesh"));
	grip = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GripMesh"));
	pommel = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PommelMesh"));
	tip = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TipMesh"));

	blade->SetupAttachment(sceneComponent);
	blade->SetRelativeLocation(sceneComponent->GetComponentLocation());

	guard->SetupAttachment(blade);
	guard->SetRelativeLocation(blade->GetComponentLocation());

	grip->SetupAttachment(guard);
	grip->SetRelativeLocation(guard->GetComponentLocation());

	pommel->SetupAttachment(grip);
	pommel->SetRelativeLocation(grip->GetComponentLocation());

	tip->SetupAttachment(blade);
	tip->SetRelativeLocation(blade->GetComponentLocation());
	
	//cook physics stuff
	blade->bUseAsyncCooking = true;
	guard->bUseAsyncCooking = true;
	grip->bUseAsyncCooking = true;
	pommel->bUseAsyncCooking = true;
	tip->bUseAsyncCooking = true;

	//Randomize the swords size parameters
	//X = left and right	(Width)
	//Y = forward and back	(Girth)
	//Z = height			(Height)

	guardGirthMultiMin = 0.6;
	guardWidthMultiMin = 0.8;


}

// Called when the game starts or when spawned
void APCGSword::BeginPlay()
{
	Super::BeginPlay();
}

void APCGSword::PostActorCreated()
{
	//Generate Mesh when created in level
	Super::PostActorCreated();
	GenerateMesh();
}

void APCGSword::PostLoad()
{
	//Generate Mesh when loaded in level
	Super::PostLoad();
	GenerateMesh();
}

// Called every frame
void APCGSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCGSword::MeshReset()
{
	//Reset all attributes. Good Practice.
	vertices.Reset();
	triangles.Reset();
	normals.Reset();
	uvs.Reset();
	vertexColors.Reset();
	tangents.Reset();
	/*
	bladeVertices.Reset();
	bladeTriangles.Reset();
	bladeNormals.Reset();
	bladeUvs.Reset();
	bladeVertexColors.Reset();
	bladeTangents.Reset();

	guardVertices.Reset();
	guardTriangles.Reset();
	guardNormals.Reset();
	guardUvs.Reset();
	guardVertexColors.Reset();
	guardTangents.Reset();*/

	gripVertices.Reset();
	gripTriangles.Reset();
	gripNormals.Reset();
	gripUvs.Reset();
	gripVertexColors.Reset();
	gripTangents.Reset();
}

void APCGSword::GenerateMesh()
{
	MeshReset();

	//Size of the cube at base
	float cubeSizeMin = 5.f;
	float cubeSizeMax = 50.f;
	float randCubeSize = FMath::RandRange(cubeSizeMin, cubeSizeMax);

	//Height
	float heightMin = 25.f;
	float heightMax = 200.f;
	randHeight = FMath::RandRange(heightMin, heightMax);

	//Set size of cube to be the randomized parameters
	bladeCubeRadius = FVector(randCubeSize, randCubeSize, randHeight);

	/// --------------------- Width and Girth for later use after mesh construction, its just here for consistensy sake ---------------------

	//Width
	//Set random width for use later after the cube is created
	float widthMin = 0.2f;
	float widthMax = 1.3f;
	width = FMath::RandRange(widthMin, widthMax);

	//Girth
	//Set random girth for use later after the cube is created
	float girthMin = widthMin;
	float girthMax = width - (width / 4); //WE GOTS A MAGIC NUMBA HERE BAWSS!!! (wait does this even count as a magic number?)
	girth = FMath::RandRange(girthMin, girthMax);

	/// --------------------- Guard Attributes ---------------------

	//Guard Width
	float guardWidthMin = randCubeSize + (randCubeSize / 7);
	float guardWidthMax = randCubeSize + (randCubeSize / 5);
	float guardWidth = FMath::RandRange(guardWidthMin, guardWidthMax);

	//Girth Girth
	float guardGirthMin = randCubeSize + (guardWidthMin / 4.f);
	float guardGirthMax = randCubeSize + (guardWidthMax / 2.f);
	float guardGirth = FMath::RandRange(guardWidthMin, guardWidthMax);

	//Height
	float guardHeightMin = 3.f;
	float guardHeightMax = 4.f;
	float guardHeight = FMath::RandRange(guardHeightMin, guardHeightMax);

	guardCubeRadius = FVector(guardWidth, guardGirth, guardHeight);

	/// --------------------- Grip Attributes ---------------------

	//Grip Width
	float gripWidthMin = 5;
	float gripWidthMax = 7;
	float gripWidth = FMath::RandRange(gripWidthMin, gripWidthMax);

	//Grip Height
	float gripHeightMin = 15;
	float gripHeightMax = 35;
	float gripHeight = FMath::RandRange(gripHeightMin, gripHeightMax);

	gripCubeRadius = FVector(gripWidth, gripWidth, gripHeight);

	/// --------------------- Pommel Attributes ---------------------
	
	//Pommel Width
	float pommelWidthMin = gripWidth+1.f;
	float pommelWidthMax = gripWidth+2.f;
	float pommelWidth = FMath::RandRange(pommelWidthMin, pommelWidthMax);

	//Pommel Height
	float pommelHeightMin = pommelWidthMin-1.f;
	float pommelHeightMax = pommelWidthMax;
	float pommelHeight = FMath::RandRange(pommelHeightMin, pommelHeightMax);

	pommelCubeRadius = FVector(pommelWidth, pommelWidth, pommelHeight);

	/// --------------------- Tip Attributes ---------------------

	//Tip Size, same lenght on all sides for now
	float tipSize = randCubeSize;

	tipCubeRadius = FVector(tipSize, tipSize, tipSize);

	//Fill the mesh variables with mesh data
	GenerateBlade();

	//Create the actual mesh from the multiple quad meshes

	//------------------------------ Modify the Blade's Transform to look... uh... blade-like ------------------------------
	blade->CreateMeshSection_LinearColor(0, bladeVertices, bladeTriangles, bladeNormals, bladeUvs, bladeVertexColors, bladeTangents, true);

	blade->SetWorldRotation(FRotator(0.f, 90.f,0.f));	//comment this properly bro
	blade->SetRelativeScale3D(FVector(girth, width, 1.f));

	//------------------------------ Generate Guard Mesh, Modify it ------------------------------
	GenerateGuard();

	guard->CreateMeshSection_LinearColor(0, guardVertices, guardTriangles, guardNormals, guardUvs, guardVertexColors, guardTangents, true);

	//clamp size of guard. girth to 0.6 as it should be smaller, width to 0.8 as it should be longer. - These are magic number and should be changed
	float guardGirthMulti = girth;
	if (guardGirthMulti < guardGirthMultiMin)
	{
		guardGirthMulti = guardGirthMultiMin;
	}

	float guardWidthMulti = width;
	if (guardWidthMulti < guardWidthMultiMin)
	{
		guardWidthMulti = guardWidthMultiMin;
	}
	
	guard->SetRelativeScale3D(FVector(guardGirthMulti * 3, guardWidthMulti * 2, 1.f));	//magic number, ill fix this later
	guard->SetWorldLocation(blade->GetComponentLocation() - (FVector(0.f,0.f, (bladeCubeRadius.Z + guardCubeRadius.Z))));

	//------------------------------ Generate Grip Mesh, Modify it ------------------------------
	GenerateGrip();
	
	grip->CreateMeshSection_LinearColor(0, gripVertices, gripTriangles, gripNormals, gripUvs, gripVertexColors, gripTangents, true);
	
	grip->SetRelativeScale3D(FVector(1.f, 0.6f, 1.f));
	grip->SetWorldLocation(guard->GetComponentLocation() - (FVector(0.f, 0.f, (guardCubeRadius.Z + gripCubeRadius.Z))));

	//------------------------------ Generate Pommel Mesh, Modify it ------------------------------
	GeneratePommel();

	pommel->CreateMeshSection_LinearColor(0, pommelVertices, pommelTriangles, pommelNormals, pommelUvs, pommelVertexColors, pommelTangents, true);

	pommel->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	pommel->SetWorldLocation(grip->GetComponentLocation() - (FVector(0.f, 0.f, (gripCubeRadius.Z + pommelCubeRadius.Z))));

	//------------------------------ Generate Tip Mesh, Modify it ------------------------------
	GenerateTip();

	tip->CreateMeshSection_LinearColor(0, tipVertices, tipTriangles, tipNormals, tipUvs, tipVertexColors, tipTangents, true);

	tip->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	tip->SetWorldLocation(blade->GetComponentLocation() + (FVector(0.f, 0.f, (bladeCubeRadius.Z + tipCubeRadius.Z+1.f))));
}

void APCGSword::GenerateBlade()
{
	MeshReset();

	int32 triangleIndexCount = 0;
	FVector definedShape[8];
	FProcMeshTangent tangentSetup;

	const FRotator rot(0, 45, 0);

	definedShape[0] = rot.RotateVector(FVector(-bladeCubeRadius.X, bladeCubeRadius.Y, bladeCubeRadius.Z));	//Forward Top Right
	definedShape[1] = rot.RotateVector(FVector(-bladeCubeRadius.X, bladeCubeRadius.Y, -bladeCubeRadius.Z));	//Forward Bottom Right
	definedShape[2] = rot.RotateVector(FVector(-bladeCubeRadius.X, -bladeCubeRadius.Y, bladeCubeRadius.Z));	//Forward Top Left
	definedShape[3] = rot.RotateVector(FVector(-bladeCubeRadius.X, -bladeCubeRadius.Y, -bladeCubeRadius.Z));//Forward Bottom Left

	definedShape[4] = rot.RotateVector(FVector(bladeCubeRadius.X, -bladeCubeRadius.Y, bladeCubeRadius.Z));	//Reverse Top Right
	definedShape[5] = rot.RotateVector(FVector(bladeCubeRadius.X, -bladeCubeRadius.Y, -bladeCubeRadius.Z));	//Reverse Bottom Right
	definedShape[6] = rot.RotateVector(FVector(bladeCubeRadius.X, bladeCubeRadius.Y, bladeCubeRadius.Z));	//Reverse Top Left
	definedShape[7] = rot.RotateVector(FVector(bladeCubeRadius.X, bladeCubeRadius.Y, -bladeCubeRadius.Z));	//Reverse Bottom Left

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

	bladeVertices = vertices;
	bladeTriangles = triangles;
	bladeNormals = normals;
	bladeUvs = uvs;
	bladeVertexColors = vertexColors;
	bladeTangents = tangents;
}

void APCGSword::GenerateGuard()
{
	MeshReset();

	int32 triangleIndexCount = 0;
	FVector definedShape[8];
	FProcMeshTangent tangentSetup;

	definedShape[0] = FVector(-guardCubeRadius.X, guardCubeRadius.Y, guardCubeRadius.Z);	//Forward Top Right
	definedShape[1] = FVector(-guardCubeRadius.X, guardCubeRadius.Y, -guardCubeRadius.Z);	//Forward Bottom Right
	definedShape[2] = FVector(-guardCubeRadius.X, -guardCubeRadius.Y, guardCubeRadius.Z);	//Forward Top Left
	definedShape[3] = FVector(-guardCubeRadius.X, -guardCubeRadius.Y, -guardCubeRadius.Z);	//Forward Bottom Left

	definedShape[4] = FVector(guardCubeRadius.X, -guardCubeRadius.Y, guardCubeRadius.Z);	//Reverse Top Right
	definedShape[5] = FVector(guardCubeRadius.X, -guardCubeRadius.Y, -guardCubeRadius.Z);	//Reverse Bottom Right
	definedShape[6] = FVector(guardCubeRadius.X, guardCubeRadius.Y, guardCubeRadius.Z);		//Reverse Top Left
	definedShape[7] = FVector(guardCubeRadius.X, guardCubeRadius.Y, -guardCubeRadius.Z);	//Reverse Bottom Left

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
	
	guardVertices = vertices;
	guardTriangles = triangles;
	guardNormals = normals;
	guardUvs = uvs;
	guardVertexColors = vertexColors;
	guardTangents = tangents;
}

void APCGSword::GenerateGrip()
{
	MeshReset();

	int32 gripTriangleIndexCount = 0;
	FVector gripDefinedShape[8];
	FProcMeshTangent gripTangentSetup;

	gripDefinedShape[0] = FVector(-gripCubeRadius.X, gripCubeRadius.Y, gripCubeRadius.Z);	//Forward Top Right
	gripDefinedShape[1] = FVector(-gripCubeRadius.X, gripCubeRadius.Y, -gripCubeRadius.Z);	//Forward Bottom Right
	gripDefinedShape[2] = FVector(-gripCubeRadius.X, -gripCubeRadius.Y, gripCubeRadius.Z);	//Forward Top Left
	gripDefinedShape[3] = FVector(-gripCubeRadius.X, -gripCubeRadius.Y, -gripCubeRadius.Z);	//Forward Bottom Left

	gripDefinedShape[4] = FVector(gripCubeRadius.X, -gripCubeRadius.Y, gripCubeRadius.Z);	//Reverse Top Right
	gripDefinedShape[5] = FVector(gripCubeRadius.X, -gripCubeRadius.Y, -gripCubeRadius.Z);	//Reverse Bottom Right
	gripDefinedShape[6] = FVector(gripCubeRadius.X, gripCubeRadius.Y, gripCubeRadius.Z);		//Reverse Top Left
	gripDefinedShape[7] = FVector(gripCubeRadius.X, gripCubeRadius.Y, -gripCubeRadius.Z);	//Reverse Bottom Left

	//Front
	gripTangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddQuadMesh(gripDefinedShape[0], gripDefinedShape[1], gripDefinedShape[2], gripDefinedShape[3], gripTriangleIndexCount, gripTangentSetup);

	//Left
	gripTangentSetup = FProcMeshTangent(1.f, 0.0f, 0.0f);
	AddQuadMesh(gripDefinedShape[2], gripDefinedShape[3], gripDefinedShape[4], gripDefinedShape[5], gripTriangleIndexCount, gripTangentSetup);

	//Back
	gripTangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddQuadMesh(gripDefinedShape[4], gripDefinedShape[5], gripDefinedShape[6], gripDefinedShape[7], gripTriangleIndexCount, gripTangentSetup);

	//Right
	gripTangentSetup = FProcMeshTangent(-1.f, 0.0f, 0.0f);
	AddQuadMesh(gripDefinedShape[6], gripDefinedShape[7], gripDefinedShape[0], gripDefinedShape[1], gripTriangleIndexCount, gripTangentSetup);

	//Top
	gripTangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddQuadMesh(gripDefinedShape[6], gripDefinedShape[0], gripDefinedShape[4], gripDefinedShape[2], gripTriangleIndexCount, gripTangentSetup);

	//Bottom
	gripTangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddQuadMesh(gripDefinedShape[1], gripDefinedShape[7], gripDefinedShape[3], gripDefinedShape[5], gripTriangleIndexCount, gripTangentSetup);

	gripVertices = vertices;
	gripTriangles = triangles;
	gripNormals = normals;
	gripUvs = uvs;
	gripVertexColors = vertexColors;
	gripTangents = tangents;
}

void APCGSword::GeneratePommel()
{
	MeshReset();

	int32 triangleIndexCount = 0;
	FVector definedShape[8];
	FProcMeshTangent tangentSetup;

	definedShape[0] = FVector(-pommelCubeRadius.X, pommelCubeRadius.Y, pommelCubeRadius.Z);	//Forward Top Right
	definedShape[1] = FVector(-pommelCubeRadius.X, pommelCubeRadius.Y, -pommelCubeRadius.Z);	//Forward Bottom Right
	definedShape[2] = FVector(-pommelCubeRadius.X, -pommelCubeRadius.Y, pommelCubeRadius.Z);	//Forward Top Left
	definedShape[3] = FVector(-pommelCubeRadius.X, -pommelCubeRadius.Y, -pommelCubeRadius.Z);	//Forward Bottom Left

	definedShape[4] = FVector(pommelCubeRadius.X, -pommelCubeRadius.Y, pommelCubeRadius.Z);	//Reverse Top Right
	definedShape[5] = FVector(pommelCubeRadius.X, -pommelCubeRadius.Y, -pommelCubeRadius.Z);	//Reverse Bottom Right
	definedShape[6] = FVector(pommelCubeRadius.X, pommelCubeRadius.Y, pommelCubeRadius.Z);		//Reverse Top Left
	definedShape[7] = FVector(pommelCubeRadius.X, pommelCubeRadius.Y, -pommelCubeRadius.Z);	//Reverse Bottom Left

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

	pommelVertices = vertices;
	pommelTriangles = triangles;
	pommelNormals = normals;
	pommelUvs = uvs;
	pommelVertexColors = vertexColors;
	pommelTangents = tangents;
}

void APCGSword::GenerateTip()
{
	MeshReset();

	int32 triangleIndexCount = 0;
	FVector definedShape[5];
	FProcMeshTangent tangentSetup;

	definedShape[0] = FVector(-tipCubeRadius.X, 0.f, -tipCubeRadius.Z);	//Middle Right
	definedShape[1] = FVector(0.f, tipCubeRadius.Y, -tipCubeRadius.Z);	//Forward Centre
	definedShape[2] = FVector(tipCubeRadius.X, 0.f, -tipCubeRadius.Z);	//Middle Left
	definedShape[3] = FVector(0.f, -tipCubeRadius.Y, -tipCubeRadius.Z);//Back Centre
	definedShape[4] = FVector(0.f, 0.f, tipCubeRadius.Z);	//Tip Centre

	//Front
	tangentSetup = FProcMeshTangent(0.f, 0.0f, 1.0f);
	AddTriangleMesh(definedShape[0], definedShape[1], definedShape[3], triangleIndexCount, tangentSetup);

	//Left
	tangentSetup = FProcMeshTangent(0.f, 0.0f, 1.0f);
	AddTriangleMesh(definedShape[1], definedShape[2], definedShape[3], triangleIndexCount, tangentSetup);

	//Back
	tangentSetup = FProcMeshTangent(1.f, 0.0f, 0.0f);
	AddTriangleMesh(definedShape[0], definedShape[1], definedShape[4], triangleIndexCount, tangentSetup);

	//Right
	tangentSetup = FProcMeshTangent(-1.f, 0.0f, 0.0f);
	AddTriangleMesh(definedShape[0], definedShape[4], definedShape[3], triangleIndexCount, tangentSetup);

	//Top
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddTriangleMesh(definedShape[1], definedShape[2], definedShape[4], triangleIndexCount, tangentSetup);

	//Bottom
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddTriangleMesh(definedShape[3], definedShape[4], definedShape[2], triangleIndexCount, tangentSetup);

	tipVertices = vertices;
	tipTriangles = triangles;
	tipNormals = normals;
	tipUvs = uvs;
	tipVertexColors = vertexColors;
	tipTangents = tangents;
}

void APCGSword::AddTriangleMesh(FVector topRight, FVector bottomRight, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent)
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

void APCGSword::AddQuadMesh(FVector topLeft, FVector bottomLeft, FVector topRight, FVector bottomRight, int32& triIndex, FProcMeshTangent tangent)
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

