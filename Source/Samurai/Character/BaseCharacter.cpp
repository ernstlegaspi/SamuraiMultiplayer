#include "BaseCharacter.h"

void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();

	CurrentHealth = 1.f;
}

bool ABaseCharacter::GetIsAttacked() {
	return bIsAttacked;
}

float ABaseCharacter::GetCurrentHealth() {
	return CurrentHealth;
}

void ABaseCharacter::SetIsAttacked(bool IsAttacked) {
	bIsAttacked = IsAttacked;
}

void ABaseCharacter::SetCurrentHealth(float Value) {
	CurrentHealth = Value;
}
