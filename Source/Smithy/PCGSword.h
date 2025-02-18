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
	float width;
	float girth;
	float randHeight;

	//------ Guard Variables ------
	//Size of Guard
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector guardCubeRadius;

	//Min number the size multiplier of the guard girth and width can be
	float guardGirthMultiMin;
	float guardWidthMultiMin;

	//Max number the size multiplier of the guard girth and width can be
	float guardGirthMultiMax;
	float guardWidthMultiMax;

	//------ Grip Variables ------
	//Size of Grip
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector gripCubeRadius;

	//------ Pommel Variables ------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector pommelCubeRadius;

	//------ Tip Variables ------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector tipCubeRadius;

	//------ Prism Variables ------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector prismCubeRadius;
	bool isPrismBladeType; //Temporary check for blade type, will make this an enum later

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

	//Grip Attributes
	TArray<FVector> gripVertices;
	TArray<int> gripTriangles;
	TArray<FVector> gripNormals;
	TArray<FVector2D> gripUvs;
	TArray<FLinearColor> gripVertexColors;
	TArray<FProcMeshTangent> gripTangents;

	//Pommel Attributes
	TArray<FVector> pommelVertices;
	TArray<int> pommelTriangles;
	TArray<FVector> pommelNormals;
	TArray<FVector2D> pommelUvs;
	TArray<FLinearColor> pommelVertexColors;
	TArray<FProcMeshTangent> pommelTangents;

	//Tip Attributes
	TArray<FVector> tipVertices;
	TArray<int> tipTriangles;
	TArray<FVector> tipNormals;
	TArray<FVector2D> tipUvs;
	TArray<FLinearColor> tipVertexColors;
	TArray<FProcMeshTangent> tipTangents;

	//Prism Attributes
	TArray<FVector> prismVertices;
	TArray<int> prismTriangles;
	TArray<FVector> prismNormals;
	TArray<FVector2D> prismUvs;
	TArray<FLinearColor> prismVertexColors;
	TArray<FProcMeshTangent> prismTangents;

	//Generate each section of the sword, somewhat of a main function
	void GenerateMesh();

	//Reset the variables for mesh generation to be used in the next part of teh sword
	void MeshReset();

	//Generate each part of the sword
	void GenerateBlade();
	void GenerateGuard();
	void GenerateGrip();
	void GeneratePommel();
	void GenerateTip();
	void GeneratePrismTip();
	void GeneratePrismBlade();

	//Generates a cube based on the 8 vectors passed in 
	void GenerateSwordCube(FVector defShape[8]);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* sceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* blade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* guard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* grip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* pommel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* tip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* prismBlade;

	void AddTriangleMesh(FVector topRight, FVector bottomRight, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent);
	void AddQuadMesh(FVector topRight, FVector bottomRight, FVector topLeft, FVector bottomLeft, int32& triIndex, FProcMeshTangent tangent);

	//Blade Height
	float heightMin;
	float heightMax;

	//Blade Width
	float cubeSizeMin;
	float cubeSizeMax;

	//Guard Width
	float guardWidthMin;
	float guardWidthMax;

	float guardWidthMultiplier;

	//Grip Height
	float gripHeightMin;
	float gripHeightMax;

	//Pommel Size
	float pommelWidthMin;
	float pommelWidthMax;

	//Blade Type
	float randBladeType;

	UPROPERTY(EditAnywhere)
	float randCubeSize;



	void SetBladeAttributes(float newMinBladeH, float newMaxBladeH, float newMinBladeW, float newMaxBladeW, float newGuardMulti, float newMinGuardW, float newMaxGuardW, float newMinGripH, float newMaxGripH, float newMinPommelSize, float newMaxPommelSize, bool isPrismBlade);
};
