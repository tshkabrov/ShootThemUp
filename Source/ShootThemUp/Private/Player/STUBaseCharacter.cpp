// Shoot Them Up


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
	
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto Health = HealthComponent->GetHealth();
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("PlayerMoveForward", this, &ASTUBaseCharacter::PlayerMoveForward);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &ASTUBaseCharacter::PlayerMoveRight);
	PlayerInputComponent->BindAxis("PlayerLookUp", this, &ASTUBaseCharacter::PlayerLookUp);
	PlayerInputComponent->BindAxis("PlayerTurnAround", this, &ASTUBaseCharacter::PlayerTurnAround);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASTUBaseCharacter::OnStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASTUBaseCharacter::OnStopSprint);

}

float ASTUBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::PlayerMoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::PlayerMoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::PlayerLookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void ASTUBaseCharacter::PlayerTurnAround(float Amount)
{
	AddControllerYawInput(Amount);
}

void ASTUBaseCharacter::OnStartSprint()
{
	IsWantsToSpring = true;
}

void ASTUBaseCharacter::OnStopSprint()
{
	IsWantsToSpring = false;
}

bool ASTUBaseCharacter::IsSprinting() const {
	return IsWantsToSpring && IsMovingForward && !GetVelocity().IsZero();
}
