// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// Create a CameraComponent	
	cameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	cameraArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(cameraArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Mapping context for movement
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(inputMappingContext, 0);
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponenet = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponenet->BindAction(moveIA, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponenet->BindAction(lookIA, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponenet->BindAction(attackIA, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
		EnhancedInputComponenet->BindAction(interactIA, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	}

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D moveVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), moveVector.Y);
	AddMovementInput(GetActorRightVector(), moveVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D lookVector = Value.Get<FVector2D>();
	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(-lookVector.Y);
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
}

