// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/Gun.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->RelativeLocation = FVector(0.f, 0.f, 60.5f);
	Camera->bUsePawnControlRotation = true;

	FPArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FPArms->SetOnlyOwnerSee(true);
	FPArms->SetupAttachment(Camera);
	FPArms->bCastDynamicShadow = false;
	FPArms->CastShadow = false;
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if (!ensure(GunBlueprint)) { return; }
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	Gun->AttachToComponent(FPArms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	Gun->AnimInstance = FPArms->GetAnimInstance();
}

void AMannequin::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void AMannequin::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMannequin::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMannequin::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMannequin::Fire()
{
	if (!Gun) { return; }
	Gun->OnFire();
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMannequin::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMannequin::StopJumping);
	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::Fire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMannequin::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMannequin::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AMannequin::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMannequin::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMannequin::LookUpAtRate);

}