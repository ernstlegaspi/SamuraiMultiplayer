#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SAMURAI_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

private:
	bool bIsAttacked;

	float CurrentHealth;

public:
	float GetCurrentHealth();

	bool GetIsAttacked();

	void SetCurrentHealth(float Value);
	void SetIsAttacked(bool IsAttacked);

protected:
	virtual void BeginPlay() override;

};
