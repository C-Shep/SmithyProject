// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SMITHY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- Movement Functions ---
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	// --- Input Actions ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* moveIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* lookIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* attackIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* interactIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* inputMappingContext;

	// --- Camera ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* cameraArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* camera;

};
