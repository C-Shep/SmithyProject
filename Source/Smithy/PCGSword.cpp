// Fill out your copyright notice in the Description page of Project Settings.

#include "PCGSword.h"
#include "Logging/LogMacros.h"
#include "Math/Vector.h"
#include "Kismetproceduralmeshlibrary.h"
#include "EngineGlobals.h"


// Sets default values
APCGSword::APCGSword()
{
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
	prismBlade = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PrismMesh"));

	blade->SetupAttachment(sceneComponent);
	blade->SetRelativeLocation(sceneComponent->GetComponentLocation());

	guard->SetupAttachment(sceneComponent);
	guard->SetRelativeLocation(sceneComponent->GetComponentLocation());

	grip->SetupAttachment(sceneComponent);
	grip->SetRelativeLocation(sceneComponent->GetComponentLocation());

	pommel->SetupAttachment(sceneComponent);
	pommel->SetRelativeLocation(sceneComponent->GetComponentLocation());

	tip->SetupAttachment(blade);
	tip->SetRelativeLocation(blade->GetComponentLocation());

	prismBlade->SetupAttachment(sceneComponent);
	prismBlade->SetRelativeLocation(sceneComponent->GetComponentLocation());
	
	//cook physics stuff
	blade->bUseAsyncCooking = true;
	guard->bUseAsyncCooking = true;
	grip->bUseAsyncCooking = true;
	pommel->bUseAsyncCooking = true;
	tip->bUseAsyncCooking = true;
	prismBlade->bUseAsyncCooking = true;

	//Randomize the swords size parameters
	//X = left and right	(Width)
	//Y = forward and back	(Girth)
	//Z = height			(Height)

	guardGirthMultiMin = 0.6f;
	guardGirthMultiMax = 1.f;

	guardWidthMultiMin = 0.8f;
	guardWidthMultiMin = 1.5f;

	//Random Stat 
	swordDamageMultLow = 0.9f;
	swordDamageMultHigh = 1.1f;

	swordSwingSpeedMultLow = 0.9f;
	swordSwingSpeedMultHigh = 1.1f;

	swordDefenceMultLow = 0.9f;
	swordDefenceMultHigh = 1.1f;

	swordWeightMultLow = 0.8f;
	swordWeightMultHigh = 1.0f;

	swordDurabilityMultLow = 1.0f;
	swordDurabilityMultHigh = 1.5f;

	steelMod = 0.5f;
	ironMod = 0.7f;
	copperMod = 0.9f;

	FillNames();
}

void APCGSword::SetBladeAttributes(float newMinBladeH, float newMaxBladeH, float newMinBladeW, float newMaxBladeW, float newGuardMulti, float newMinGuardW, float newMaxGuardW, float newMinGripH, float newMaxGripH, float newMinPommelSize, float newMaxPommelSize, int newBladeType, float newCurve, int newMatType)
{
	//Blade H
	heightMin = newMinBladeH;
	heightMax = newMaxBladeH;

	//Blade W
	cubeSizeMin = newMinBladeW;
	cubeSizeMax = newMaxBladeW;

	//Guard Width
	guardWidthMin = newMinGuardW;
	guardWidthMax = newMaxGuardW;

	guardWidthMultiplier = newGuardMulti;

	//Grip Height
	gripHeightMin = newMinGripH;
	gripHeightMax = newMaxGripH;

	//Pommel Size
	pommelWidthMin = newMinPommelSize;
	pommelWidthMax = newMaxPommelSize;

	//Blade Type
	bladeType = newBladeType;

	//Curve
	curveAmount = newCurve;

	//Mat Type
	matType = newMatType;
}

// Called when the game starts or when spawned
void APCGSword::BeginPlay()
{
	Super::BeginPlay();
	GenerateMesh();
}

void APCGSword::PostActorCreated()
{
	//Generate Mesh when created in level
	Super::PostActorCreated();
//	GenerateMesh();
}

void APCGSword::PostLoad()
{
	//Generate Mesh when loaded in level
	Super::PostLoad();
	//GenerateMesh();
}

// Called every frame
void APCGSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCGSword::MeshReset()
{
	//Reset all default attributes. Not required but good practice. i think.
	vertices.Reset();
	triangles.Reset();
	normals.Reset();
	uvs.Reset();
	vertexColors.Reset();
	tangents.Reset();
}

void APCGSword::GenerateMesh()
{
	MeshReset();

	//Set Base Rotation for the Sword, 90 so it faces the Camera (change this back if prism blade tho)
	this->SetActorRotation(FRotator(0.f,90.f,0.f));

	/// --------------------- Blade Attributes ---------------------
	
	//Size of the cube at base
	randCubeSize = FMath::RandRange(cubeSizeMin, cubeSizeMax);

	//Random height of the blade
	randHeight = FMath::RandRange(heightMin, heightMax);

	//Set size of cube to be the randomized parameters
	bladeCubeRadius = FVector(randCubeSize, randCubeSize, randHeight);

	/// --------------------- Width and Girth for later use after mesh construction, its just here for consistensy sake ---------------------

	//Width
	//Set random width for use later after the cube is created
	const float widthMin = 0.5f;
	const float widthMax = 1.1f;
	width = FMath::RandRange(widthMin, widthMax);

	//Girth
	//Set random girth for use later after the cube is created
	const float girthMin = widthMin;
	const float girthMax = width - (width / 4); //WE GOTS A MAGIC NUMBA HERE BAWSS!!! (wait does this even count as a magic number?)
	girth = FMath::RandRange(girthMin, girthMax);

	/// --------------------- Guard Attributes ---------------------

	//Guard Width
	float guardWidth = FMath::RandRange(guardWidthMin, guardWidthMax);

	if (guardWidth < (randCubeSize - (randCubeSize/2))) guardWidth = randCubeSize - (randCubeSize / 2);

	//Girth Girth
	const float guardGirthMin = randCubeSize;
	const float guardGirthMax = randCubeSize;
	const float guardGirth = FMath::RandRange(guardGirthMin, guardGirthMax);

	//Height
	const float guardHeightMin = 3.f;
	const float guardHeightMax = 4.f;
	const float guardHeight = FMath::RandRange(guardHeightMin, guardHeightMax);

	//X is towards camera, Y is left and right
	guardCubeRadius = FVector(guardGirth, guardWidth, guardHeight);

	/// --------------------- Grip Attributes ---------------------

	//Grip Width
	const float gripWidthMin = 5;
	const float gripWidthMax = 7;
	const float gripWidth = FMath::RandRange(gripWidthMin, gripWidthMax);

	//Grip Height
	//const float gripHeightMin = 15;
	//const float gripHeightMax = 35;
	const float gripHeight = FMath::RandRange(gripHeightMin, gripHeightMax);

	gripCubeRadius = FVector(gripWidth, gripWidth, gripHeight);

	/// --------------------- Pommel Attributes ---------------------
	
	//Pommel Width
	const float pommelWidth = FMath::RandRange(pommelWidthMin, pommelWidthMax);

	//Pommel Height
	const float pommelHeightMin = pommelWidth;
	const float pommelHeightMax = pommelWidth;
	const float pommelHeight = FMath::RandRange(pommelHeightMin, pommelHeightMax);

	pommelCubeRadius = FVector(pommelWidth, pommelWidth, pommelHeight);

	/// --------------------- Tip Attributes ---------------------

	//Tip Size, same lenght on all sides for now
	const float tipSize = randCubeSize;

	tipCubeRadius = FVector(tipSize, tipSize, tipSize);

	/// --------------------- Prism Attributes ---------------------

	//Prism Size, same lenght on all sides for now
	const float prismSize = randCubeSize;

	prismCubeRadius = FVector(prismSize, prismSize, randHeight);

	/// --------------------- Curve Attributes ---------------------

	//Prism Size, same lenght on all sides for now
	const float curveSize = randCubeSize;

	curveCubeRadius = FVector(curveSize, curveSize, randHeight);

	//------------------------------ Fill the Mesh Variables with Mesh Data ------------------------------

	//------------------------------ Generate Blade Mesh, Modify it ------------------------------

	GenerateBlade();

	//Rotate Blade Mesh for Normal Blade
	if (bladeType == 0 || bladeType == 1)
	{
		blade->CreateMeshSection_LinearColor(0, bladeVertices, bladeTriangles, bladeNormals, bladeUvs, bladeVertexColors, bladeTangents, true);
		blade->SetWorldRotation(FRotator(0.f, 0.f, 0.f));	
	}

	if (bladeType == 1)
	{
		blade->CreateMeshSection_LinearColor(0, bladeVertices, bladeTriangles, bladeNormals, bladeUvs, bladeVertexColors, bladeTangents, true);
		width = width / 4;
		blade->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
	}

	//Rotate Blade Mesh for Prism Blade
	if(bladeType == 2) 
	{
		for (int i = 0; i < 9; i++)
		{
			blade->CreateMeshSection_LinearColor(
				i,
				curveVertices[i],
				curveTriangles[i],
				curveNormals[i],
				curveUvs[i],
				curveVertexColors[i],
				curveTangents[i],
				true
			);
		}
		blade->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
	}

	float matBladeScale = FMath::RandRange(-0.05f, 0.05f);
	blade->SetRelativeScale3D(FVector(girth, width/4, 1.f));

	//Calculate blade volume, make the cube radius matter more cuz its a smaller number than the height
	bladeVolume = (bladeCubeRadius.X * 1.2) * (bladeCubeRadius.Y * 1.2) * randHeight;

	//------------------------------ Generate Guard Mesh, Modify it ------------------------------
	GenerateGuard();

	guard->CreateMeshSection_LinearColor(0, guardVertices, guardTriangles, guardNormals, guardUvs, guardVertexColors, guardTangents, true);

	//clamp size of guard. girth to 0.6 as it should be smaller, width to 0.8 as it should be longer. - These are magic number and should be changed
	float guardGirthMulti = guardWidthMultiplier;
	float guardWidthMulti = guardWidthMultiplier;

	//X is towards camera, Y is left and right
	float matGuardScale = FMath::RandRange(-0.05f, 0.05f);

	guard->SetRelativeScale3D(FVector(1.f + matGuardScale, 2.f + matGuardScale, 1.f + matGuardScale));
	guard->SetWorldLocation(blade->GetComponentLocation() - (FVector(0.f,0.f, (bladeCubeRadius.Z + guardCubeRadius.Z))));

	//Calculate guard volume
	guardVolume = guardCubeRadius.X * guardCubeRadius.Y * guardCubeRadius.Z;

	//------------------------------ Generate Grip Mesh, Modify it ------------------------------
	GenerateGrip();
	
	float matGripScale = FMath::RandRange(-0.05f, 0.05f);

	grip->CreateMeshSection_LinearColor(0, gripVertices, gripTriangles, gripNormals, gripUvs, gripVertexColors, gripTangents, true);
	
	grip->SetRelativeScale3D(FVector(0.9f+ matGripScale, 0.8f + matGripScale, 1.f));
	grip->SetWorldLocation(guard->GetComponentLocation() - (FVector(0.f, 0.f, (guardCubeRadius.Z + gripCubeRadius.Z))));

	//Calculate grip volume
	gripVolume = gripCubeRadius.X * gripCubeRadius.Y * gripCubeRadius.Z;

	//------------------------------ Generate Pommel Mesh, Modify it ------------------------------
	GeneratePommel();

	pommel->CreateMeshSection_LinearColor(0, pommelVertices, pommelTriangles, pommelNormals, pommelUvs, pommelVertexColors, pommelTangents, true);

	pommel->SetRelativeScale3D(FVector(1.3f, 1.3f, 1.f));
	pommel->SetWorldLocation(grip->GetComponentLocation() - (FVector(0.f, 0.f, (gripCubeRadius.Z + pommelCubeRadius.Z))));

	//Calculate pommel volume
	pommelVolume = pommelCubeRadius.X * pommelCubeRadius.Y * pommelCubeRadius.Z;

	//------------------------------ Generate Tip Mesh, Modify it ------------------------------
	tip->SetActive(true);
	tip->SetVisibility(true);
	//A number to multiply the horizontal size of the tip to match the size of the blade. I don't know why this needs to happen but it does.
	float tipToBladeConstant = 1.415f;
	
	//Use the correct tip for hte correct sword
	if (bladeType == 0)
	{
		GenerateTip();
		tip->SetRelativeScale3D(FVector(tipToBladeConstant, tipToBladeConstant, 1.f));
	}
	else if(bladeType == 1) {
		GeneratePrismTip();
	}

	tip->CreateMeshSection_LinearColor(0, tipVertices, tipTriangles, tipNormals, tipUvs, tipVertexColors, tipTangents, true);
	tip->SetWorldLocation(blade->GetComponentLocation() + (FVector(0.f, 0.f, (bladeCubeRadius.Z + tipCubeRadius.Z))));

	//------------------------------ Calculate Stats ------------------------------
	CalculateStats();
}

void APCGSword::GenerateBlade()
{
	MeshReset();

	if(bladeType == 0)
	{
		int32 triangleIndexCount = 0;
		FVector definedShape[9];
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
		definedShape[8] = rot.RotateVector(FVector(0.f, 0.f, bladeCubeRadius.Z+50.f));	//Centre

		//Generate a cube using the defined shape array
		GenerateSwordCube(definedShape);

		AddTriangleMesh(definedShape[8], definedShape[2], definedShape[0], triangleIndexCount, tangentSetup);
		AddTriangleMesh(definedShape[8], definedShape[0], definedShape[6], triangleIndexCount, tangentSetup);
		AddTriangleMesh(definedShape[8], definedShape[6], definedShape[4], triangleIndexCount, tangentSetup);
		AddTriangleMesh(definedShape[8], definedShape[4], definedShape[2], triangleIndexCount, tangentSetup);

	}
	else if (bladeType == 1)
	{
		GeneratePrismBlade();
	}
	else if (bladeType == 2)
	{
		GenerateCurvedBlade();
	}

	bladeVertices = vertices;
	bladeTriangles = triangles;
	bladeNormals = normals;
	bladeUvs = uvs;

	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(bladeVertices, bladeTriangles, bladeUvs, bladeNormals, bladeTangents);

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

	//Generate a cube using the defined shape array
	GenerateSwordCube(definedShape);

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
	FVector definedShape[8];
	FProcMeshTangent gripTangentSetup;

	definedShape[0] = FVector(-gripCubeRadius.X, gripCubeRadius.Y, gripCubeRadius.Z);	//Forward Top Right
	definedShape[1] = FVector(-gripCubeRadius.X, gripCubeRadius.Y, -gripCubeRadius.Z);	//Forward Bottom Right
	definedShape[2] = FVector(-gripCubeRadius.X, -gripCubeRadius.Y, gripCubeRadius.Z);	//Forward Top Left
	definedShape[3] = FVector(-gripCubeRadius.X, -gripCubeRadius.Y, -gripCubeRadius.Z);	//Forward Bottom Left

	definedShape[4] = FVector(gripCubeRadius.X, -gripCubeRadius.Y, gripCubeRadius.Z);	//Reverse Top Right
	definedShape[5] = FVector(gripCubeRadius.X, -gripCubeRadius.Y, -gripCubeRadius.Z);	//Reverse Bottom Right
	definedShape[6] = FVector(gripCubeRadius.X, gripCubeRadius.Y, gripCubeRadius.Z);		//Reverse Top Left
	definedShape[7] = FVector(gripCubeRadius.X, gripCubeRadius.Y, -gripCubeRadius.Z);	//Reverse Bottom Left

	//Generate a cube using the defined shape array
	GenerateSwordCube(definedShape);

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

	//Generate a cube using the defined shape array
	GenerateSwordCube(definedShape);

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

	//Dont use generateSwordCube for this one, its not a cube its a pyramid

	//Back Right
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddTriangleMesh(definedShape[4], definedShape[0], definedShape[1], triangleIndexCount, tangentSetup);

	///Back Left
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddTriangleMesh(definedShape[4], definedShape[3], definedShape[0], triangleIndexCount, tangentSetup);

	//Front Left
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddTriangleMesh(definedShape[4], definedShape[1], definedShape[2], triangleIndexCount, tangentSetup);

	//Front Right
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddTriangleMesh(definedShape[4], definedShape[2], definedShape[3], triangleIndexCount, tangentSetup);


	tipVertices = vertices;
	tipTriangles = triangles;
	tipUvs = uvs;

//	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(tipVertices, tipTriangles, tipUvs, tipNormals, tipTangents);

	tipNormals = normals;
	
	tipVertexColors = vertexColors;
	tipTangents = tangents;
}

void APCGSword::GeneratePrismTip()
{
	MeshReset();

	int32 triangleIndexCount = 0;
	FVector definedShape[4];
	FProcMeshTangent tangentSetup;

	definedShape[0] = FVector(-tipCubeRadius.X, 0.f, -tipCubeRadius.Z);	//Middle Left
	definedShape[1] = FVector(tipCubeRadius.X, tipCubeRadius.Y, -tipCubeRadius.Z);	//Forward Right
	definedShape[2] = FVector(tipCubeRadius.X, -tipCubeRadius.Y, -tipCubeRadius.Z);	//Back Right
	definedShape[3] = FVector(tipCubeRadius.X, 0.f, tipCubeRadius.Z); //Top

	//Dont use generateSwordCube for this one, its not a cube its a pyramid prism thing. geometry is for nerds

	//Front
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddTriangleMesh(definedShape[0], definedShape[1], definedShape[3], triangleIndexCount, tangentSetup);

	///Back
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddTriangleMesh(definedShape[2], definedShape[0], definedShape[3], triangleIndexCount, tangentSetup);

	//Right
	tangentSetup = FProcMeshTangent(1.f, 1.0f, 1.0f);
	AddTriangleMesh(definedShape[1], definedShape[2], definedShape[3], triangleIndexCount, tangentSetup);

	tipVertices = vertices;
	tipTriangles = triangles;
	tipNormals = normals;
	tipUvs = uvs;
	tipVertexColors = vertexColors;
	tipTangents = tangents;
}

void APCGSword::GeneratePrismBlade()
{
	//MeshReset();

	int32 triangleIndexCount = 0;
	FVector definedShape[6];
	FProcMeshTangent tangentSetup;

	definedShape[0] = FVector(-prismCubeRadius.X, 0.f, -prismCubeRadius.Z);	
	definedShape[1] = FVector(prismCubeRadius.X, prismCubeRadius.Y, -prismCubeRadius.Z);	
	definedShape[2] = FVector(prismCubeRadius.X, -prismCubeRadius.Y, -prismCubeRadius.Z);	
	definedShape[3] = FVector(-prismCubeRadius.X, 0.f, prismCubeRadius.Z);	
	definedShape[4] = FVector(prismCubeRadius.X, prismCubeRadius.Y, prismCubeRadius.Z);	
	definedShape[5] = FVector(prismCubeRadius.X, -prismCubeRadius.Y, prismCubeRadius.Z);	

	//Front
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddQuadMesh(definedShape[2], definedShape[5], definedShape[0], definedShape[3], triangleIndexCount, tangentSetup);

	//Right
	tangentSetup = FProcMeshTangent(1.f, 0.0f, 0.0f);
	AddQuadMesh(definedShape[3], definedShape[4], definedShape[0], definedShape[1], triangleIndexCount, tangentSetup);

	//Back
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddQuadMesh(definedShape[4], definedShape[5], definedShape[1], definedShape[2], triangleIndexCount, tangentSetup);

	prismVertices = vertices;
	prismTriangles = triangles;
	prismNormals = normals;
	prismUvs = uvs;
	prismVertexColors = vertexColors;
	prismTangents = tangents;
}

void APCGSword::GenerateCurvedBlade()
{
	//MeshReset();

	int32 triangleIndexCount = 0;
	FVector definedShape[8];
	FProcMeshTangent tangentSetup;

	FVector b = blade->GetComponentLocation();

	FVector controlPoints[4] = {
		FVector(0.f,		0.f,		-randHeight),
		FVector(0.f,		0.f,		-randHeight/2),
		FVector(curveAmount/4,		0.f,		randHeight/2),
		FVector(curveAmount,		 0.f,		randHeight)
	};

	int numPoints = 10;
	TArray<FVector> pointsOnCurve;

	FVector::EvaluateBezier(controlPoints,numPoints,pointsOnCurve);

	int numRows = pointsOnCurve.Num() - 1;
	curveVertices.SetNum(numRows);
	curveTriangles.SetNum(numRows);
	curveNormals.SetNum(numRows);
	curveUvs.SetNum(numRows);
	curveVertexColors.SetNum(numRows);
	curveTangents.SetNum(numRows);

	for (int i = 0; i < numRows; i++)
	{
		float pX = pointsOnCurve[i].X;
		float pZ = pointsOnCurve[i].Z;
		float pX2 = pointsOnCurve[i+1].X;
		float pZ2 = pointsOnCurve[i+1].Z;

		if (i < numRows - 1)
		{
			definedShape[0] = (FVector(pX2 - curveCubeRadius.X, 0.f, pZ2));		//Forward Top Right
			definedShape[1] = (FVector(pX - curveCubeRadius.X, 0.f, pZ));		//Forward Bottom Right
			definedShape[2] = (FVector(pX2 - curveCubeRadius.X, 0.f, pZ2));	//Forward Top Left
			definedShape[3] = (FVector(pX - curveCubeRadius.X, 0.f, pZ));	//Forward Bottom Left

			definedShape[4] = (FVector(pX2 + curveCubeRadius.X, -curveCubeRadius.Y, pZ2));	//Reverse Top Right
			definedShape[5] = (FVector(pX + curveCubeRadius.X, -curveCubeRadius.Y, pZ));	//Reverse Bottom Right
			definedShape[6] = (FVector(pX2 + curveCubeRadius.X, curveCubeRadius.Y, pZ2));		//Reverse Top Left
			definedShape[7] = (FVector(pX + curveCubeRadius.X, curveCubeRadius.Y, pZ));		//Reverse Bottom Left

			GenerateSwordCube(definedShape);
		}
		else {
			definedShape[0] = FVector(pX- curveCubeRadius.X, 0.f, pZ);	//Middle Left
			definedShape[1] = FVector(pX+ curveCubeRadius.X, curveCubeRadius.Y, pZ);	//Forward Right
			definedShape[2] = FVector(pX+ curveCubeRadius.X, -curveCubeRadius.Y, pZ);	//Back Right
			definedShape[3] = FVector(pX2+ curveCubeRadius.X, 0.f, pZ2); //Top

			//Dont use generateSwordCube for this one, its not a cube its a pyramid prism thing. geometry is for nerds

			//Front
			tangentSetup = FProcMeshTangent(-1.f, -1.0f,-1.0f);
			AddTriangleMesh(definedShape[3], definedShape[0], definedShape[1], triangleIndexCount, tangentSetup);

			///Back
			tangentSetup = FProcMeshTangent(-1.f, -1.0f,-1.0f);
			AddTriangleMesh(definedShape[3], definedShape[2], definedShape[0], triangleIndexCount, tangentSetup);

			//Right
			tangentSetup = FProcMeshTangent(-1.f, -1.0f, -1.0f);
			AddTriangleMesh(definedShape[3], definedShape[1], definedShape[2], triangleIndexCount, tangentSetup);
		}

		curveVertices[i] = vertices;
		curveTriangles[i] = triangles;
		curveNormals[i] = normals;
		curveUvs[i] = uvs;
		curveVertexColors[i] = vertexColors;
		curveTangents[i] = tangents;

		MeshReset();
	}
}

void APCGSword::GenerateSwordCube(FVector defShape[8])
{
	FProcMeshTangent tangentSetup;
	int32 triangleIndexCount = 0;

	//Front
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddQuadMesh(defShape[0], defShape[1], defShape[2], defShape[3], triangleIndexCount, tangentSetup);

	//Left
	tangentSetup = FProcMeshTangent(1.f, 0.0f, 0.0f);
	AddQuadMesh(defShape[2], defShape[3], defShape[4], defShape[5], triangleIndexCount, tangentSetup);

	//Back
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddQuadMesh(defShape[4], defShape[5], defShape[6], defShape[7], triangleIndexCount, tangentSetup);

	//Right
	tangentSetup = FProcMeshTangent(-1.f, 0.0f, 0.0f);
	AddQuadMesh(defShape[6], defShape[7], defShape[0], defShape[1], triangleIndexCount, tangentSetup);

	//Top
	tangentSetup = FProcMeshTangent(0.f, 1.0f, 0.0f);
	AddQuadMesh(defShape[6], defShape[0], defShape[4], defShape[2], triangleIndexCount, tangentSetup);

	//Bottom
	tangentSetup = FProcMeshTangent(0.f, -1.0f, 0.0f);
	AddQuadMesh(defShape[1], defShape[7], defShape[3], defShape[5], triangleIndexCount, tangentSetup);
}

void APCGSword::AddTriangleMesh(FVector topLeft, FVector bottomRight, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent)
{
	int32 point1 = triIndex++;
	int32 point2 = triIndex++;
	int32 point3 = triIndex++;

	vertices.Add(topLeft);
	vertices.Add(bottomRight);
	vertices.Add(bottomLeft);

	triangles.Add(point1);
	triangles.Add(point2);
	triangles.Add(point3);

	FVector edge1 = topLeft - bottomLeft;
	FVector edge2 = bottomRight - topLeft;

	FVector thisNorm = FVector::CrossProduct(edge2, edge1).GetSafeNormal();

	for (int i = 0; i < 3; i++)
	{
		normals.Add(thisNorm);
		tangents.Add(tangent);
		vertexColors.Add(FLinearColor::Red);
	}

	uvs.Add(FVector2D(0.0f, 0.0f));//Top Left
	uvs.Add(FVector2D(0.0f, 1.0f));//Bottom Left
	uvs.Add(FVector2D(1.0f, 0.0f));//Bottom Right
	
	
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

	//normals cross product, gets the perpendicular vector for normal
	FVector thisNorm = FVector::CrossProduct(topLeft - bottomRight, topLeft - topRight).GetSafeNormal();

	//set normals, tangent and colour for each point
	//4 for quad
	for (int i = 0; i < 4; i++)
	{
		normals.Add(thisNorm);
		tangents.Add(tangent);
		vertexColors.Add(FLinearColor::Red);
	}

	//uvs
	uvs.Add(FVector2D(0.0f, 1.0f));//Top Left
	uvs.Add(FVector2D(0.0f, 0.0f));//Bottom Left
	uvs.Add(FVector2D(1.0f, 1.0f));//Top Right
	uvs.Add(FVector2D(1.0f, 0.0f));//Bottom Right
}


void APCGSword::FillNames()
{
	prefixes = TArray<FString>{"The", "Great", "Hero's", "Mighty", "Legendary", "Cursed", "Oh, Mighty", "The Great", "Ol'"};
	lightNames = TArray<FString>{ "Dagger", "Stabber", "Needleblade", "Needle", "Lightsword", "Fang", "Piercer" , "Gobslayer" };
	mediumNames = TArray<FString>{ "Slasher", "Slicer", "Blade", "Sabre", "Claymore", "Starsword", "Doomsword", "Wyrmslayer" };
	heavyNames = TArray<FString>{ "Crusher", "Slayer", "Throngler", "Masher", "Dragonslayer", "Destroyer", "Slab", "Wrecker"};
}

void APCGSword::CalculateStats()
{
	//Damage Calculation
	float randomDamageMult = FMath::RandRange(swordDamageMultLow, swordDamageMultHigh);

	const int32 baseDamage = 10;
	const float damagePower = 0.5;

	swordDamage = (baseDamage + pow(bladeVolume, damagePower)) * randomDamageMult;

	//Swing Speed Calculation
	float randomSwingMult = FMath::RandRange(swordSwingSpeedMultLow, swordSwingSpeedMultHigh);

	const int32 baseSwing = 1;
	const float swingPower = 0.2;

	swordSwingSpeedFloat = baseSwing / pow(bladeVolume,swingPower) * randomSwingMult;
	swordSwingSpeedFloat = swordSwingSpeedFloat *100.f;
	swordSwingSpeed = swordSwingSpeedFloat;

	//Defence Calculations
	float randomDefMult = FMath::RandRange(swordDefenceMultLow, swordDefenceMultHigh);

	const int32 baseDef = 4;
	const float defPower = 0.5;

	swordDefence = (baseDef + pow(guardVolume, defPower)) * randomDefMult;

	//Durability Calculations
	float randomDurabilityMult = FMath::RandRange(swordDurabilityMultLow, swordDurabilityMultHigh);

	const int32 baseDurability = 100;
	const float durabilityPower = 0.6;

	float sizeBladeDurability = baseDurability / pow(bladeVolume, durabilityPower);

	swordDurabilityFloat = (baseDurability + (sizeBladeDurability * 100.f) + pow(gripVolume, durabilityPower) + pow(pommelVolume, durabilityPower)) * randomDurabilityMult;
	swordDurability = swordDurabilityFloat;
	swordDurabilityMax = swordDurability;

	//Weight Calculations (The weight is actually the mass of the sword, just calling it weight for simplicities sake)
	float randomWeightMult = FMath::RandRange(swordWeightMultLow, swordWeightMultHigh);

	const float baseWeight = 68000.f;
	const float weightPower = 1.1;

	float bladeLowVol = pow(bladeVolume, weightPower);
	float materialMod = 1.f;
	switch (matType)
	{
	case 0:
		materialMod = steelMod;
		prefixes.Add("Steel");
		prefixes.Add("Stainless Steel");
		prefixes.Add("Sheffield Steel");
		prefixes.Add("Perfect");
		break;
	case 1:
		materialMod = ironMod;
		prefixes.Add("Iron");
		prefixes.Add("Cast Iron");
		prefixes.Add("Rusted");
		prefixes.Add("Unalloyed");
		break;
	case 2:
		materialMod = copperMod;
		prefixes.Add("Copper");
		break;
	default:
		materialMod = steelMod;
		break;
	}

	swordWeight = (((baseWeight + bladeLowVol + guardVolume * 2.f + gripVolume * 2.f + pommelVolume * 2.f) * randomWeightMult) * materialMod) / 45300.f;
	swordWeightString = FString::Printf(TEXT("%.2f"), swordWeight);
	swordWeightString += "lb";

	TArray<FString> swordNameWeightArray;

	if (swordWeight > 8.f)
	{
		weightClassString = "Heavy";
		prefixes.Add("Big");
		prefixes.Add("Mega");
		swordNameWeightArray = heavyNames;
	}
	else if (swordWeight > 3.f)
	{
		weightClassString = "Medium";
		swordNameWeightArray = mediumNames;
	}
	else {
		weightClassString = "Light";
		prefixes.Add("Lil");
		prefixes.Add("Wee");
		prefixes.Add("Little");
		prefixes.Add("The Little");
		swordNameWeightArray = lightNames;
	}

	//Name Generation
	int32 prefixNum = FMath::RandRange(0, prefixes.Num()-1);
	FString chosenPrefix = prefixes[prefixNum];

	int32 swordNameNum = FMath::RandRange(0, swordNameWeightArray.Num()-1);
	FString chosenWeightName = swordNameWeightArray[swordNameNum];

	swordName = chosenPrefix + " " + chosenWeightName;
}

void APCGSword::SetDurability(int32 newDurability)
{
	swordDurability = newDurability;
}

int32 APCGSword::GetDamage()
{
	return swordDamage;
}

int32 APCGSword::GetDurability()
{
	return swordDurability;
}

int32 APCGSword::GetSwingSpeed()
{
	return swordSwingSpeed;
}

int32 APCGSword::GetMaxDurability()
{
	return swordDurabilityMax;
}

FString APCGSword::GetWeight()
{
	return swordWeightString;
}

FString APCGSword::GetWeightClass()
{
	return weightClassString;
}

FString APCGSword::GetName()
{
	return swordName;
}

int32 APCGSword::GetMatType()
{
	return matType;
}

FVector APCGSword::GetBladeCube()
{
	return bladeCubeRadius;
}

FVector APCGSword::GetGuardCube()
{
	return guardCubeRadius;
}

FVector APCGSword::GetGripCube()
{
	return gripCubeRadius;
}
