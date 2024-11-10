// Fill out your copyright notice in the Description page of Project Settings.


#include "MySword.h"

AMySword::AMySword()
{
	
}

void AMySword::PostActorCreated()
{
	//Generate Mesh when created in level
	Super::PostActorCreated();
	//GenerateMesh();
}

void AMySword::PostLoad()
{
	//Generate Mesh when loaded in level
	Super::PostLoad();
	//GenerateMesh();
}
