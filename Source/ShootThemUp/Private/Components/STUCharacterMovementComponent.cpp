// Shoot Them Up


#include "Components/STUCharacterMovementComponent.h"
#include "STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ASTUBaseCharacter* PlayerCharacter = Cast<ASTUBaseCharacter>(GetPawnOwner());
	return PlayerCharacter && PlayerCharacter->IsSprinting() ? MaxSpeed * RunModifier : MaxSpeed;
}
