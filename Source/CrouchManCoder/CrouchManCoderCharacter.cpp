// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrouchManCoderCharacter.h"

#include "CrouchManCoderMySaveGame.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACrouchManCoderCharacter

ACrouchManCoderCharacter::ACrouchManCoderCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	SaveSlot = "Whatever";

}

void ACrouchManCoderCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACrouchManCoderCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACrouchManCoderCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACrouchManCoderCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACrouchManCoderCharacter::DoCrouch);

		// Looking
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACrouchManCoderCharacter::DoUnCrouch);

		//Clicking
		EnhancedInputComponent->BindAction(SaveAction, ETriggerEvent::Started, this, &ACrouchManCoderCharacter::Save);

		EnhancedInputComponent->BindAction(LoadAction, ETriggerEvent::Started, this, &ACrouchManCoderCharacter::Load);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACrouchManCoderCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACrouchManCoderCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACrouchManCoderCharacter::DoCrouch(const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Warning, TEXT("DoCrouch '%s'"), *GetNameSafe(this));

	if (Controller != nullptr)
	{
		Crouch();
	}

}

void ACrouchManCoderCharacter::DoUnCrouch(const FInputActionValue & Value)
{
	UE_LOG(LogTemplateCharacter, Warning, TEXT("DoCrouch '%s'"), *GetNameSafe(this));

	if (Controller != nullptr)
	{
		UnCrouch();
	}
}

void ACrouchManCoderCharacter::Clicked(const FInputActionValue&)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> ignoreActors;
	//ignoreActors.Add()

	TArray<AActor*> hitActors;
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		300,
		objectTypes,
		nullptr,
		ignoreActors,
		hitActors
		
		);

	//Finally iterate over the outActor array
	// for (AActor* overlappedActor : hitActors)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
	//
	// 	auto* mesh = overlappedActor->GetComponentByClass<UStaticMeshComponent>();
	// 	if (mesh != nullptr)
	// 	{
	// 		for(int i = 0; i <mesh->GetNumMaterials(); i++)
	// 		{
	// 			mesh->SetMaterial(i, NewMaterial[i]);
	// 		}
	// 	}
	// }
}

void ACrouchManCoderCharacter::Save(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Saving Game"));

	auto saveGameExists = UGameplayStatics::DoesSaveGameExist(SaveSlot, 0);

	if(saveGameExists)
	{
		UE_LOG(LogTemp, Log, TEXT("Save Game exists, overwriting"))
	}

	auto* saveGame = UGameplayStatics::CreateSaveGameObject(UCrouchManCoderMySaveGame::StaticClass());
	auto* convertedSaveGame = Cast<UCrouchManCoderMySaveGame>(saveGame);

	convertedSaveGame->PlayerTransform = GetActorTransform();

	auto wasSaved = UGameplayStatics::SaveGameToSlot(convertedSaveGame, SaveSlot, 0);

	if(wasSaved)
	{
		UE_LOG(LogTemp, Log, TEXT("Game saved succesfully"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game Save Failed"));
	}
}

void ACrouchManCoderCharacter::Load(const FInputActionValue & Value)
{
	UE_LOG(LogTemp, Log, TEXT("Loading Game"));

	auto saveGameExists = UGameplayStatics::DoesSaveGameExist(SaveSlot, 0);

	if(!saveGameExists)
	{
		UE_LOG(LogTemp, Log, TEXT("Save Game does not exist, cancelling"));
		return;
	}

	auto* loadedSaveGame = UGameplayStatics::LoadGameFromSlot(SaveSlot, 0);
	auto* convertedLoadedSaveGame = Cast<UCrouchManCoderMySaveGame>(loadedSaveGame);



	auto wasLoaded = this->SetActorTransform(convertedLoadedSaveGame-> PlayerTransform);

	if(wasLoaded)
	{
		UE_LOG(LogTemp, Log, TEXT("Game loaded succesfully"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game load Failed"));
	}
}